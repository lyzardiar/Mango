//
//  Created by yiyang.tang on 2017/9/4.
//

#ifndef GPUParticleNode_h
#define GPUParticleNode_h

#include "cocos2d.h"

USING_NS_CC;

#define MAX_VERTICLES (1<<16)

struct GPUParticleData {
public:
    float* posx;
    float* posy;
    float* startPosX;
    float* startPosY;

    float* colorR;
    float* colorG;
    float* colorB;
    float* colorA;

    float* deltaColorR;
    float* deltaColorG;
    float* deltaColorB;
    float* deltaColorA;

    float* size;
    float* deltaSize;
    float* rotation;
    float* deltaRotation;
    float* timeToLive;
    unsigned int* atlasIndex;

    //! Mode A: gravity, direction, radial accel, tangential accel
    struct{
        float* dirX;
        float* dirY;
        float* radialAccel;
        float* tangentialAccel;
    } modeA;

    //! Mode B: radius mode
    struct{
        float* angle;
        float* degreesPerSecond;
        float* radius;
        float* deltaRadius;
    } modeB;

    unsigned int maxCount;
    GPUParticleData();
    bool init(int count);
    void release();
    unsigned int getMaxCount() { return maxCount; }

    void copyParticle(int p1, int p2)
    {
        posx[p1] = posx[p2];
        posy[p1] = posy[p2];
        startPosX[p1] = startPosX[p2];
        startPosY[p1] = startPosY[p2];

        colorR[p1] = colorR[p2];
        colorG[p1] = colorG[p2];
        colorB[p1] = colorB[p2];
        colorA[p1] = colorA[p2];

        deltaColorR[p1] = deltaColorR[p2];
        deltaColorG[p1] = deltaColorG[p2];
        deltaColorB[p1] = deltaColorB[p2];
        deltaColorA[p1] = deltaColorA[p2];

        size[p1] = size[p2];
        deltaSize[p1] = deltaSize[p2];

        rotation[p1] = rotation[p2];
        deltaRotation[p1] = deltaRotation[p2];

        timeToLive[p1] = timeToLive[p2];

        atlasIndex[p1] = atlasIndex[p2];

        modeA.dirX[p1] = modeA.dirX[p2];
        modeA.dirY[p1] = modeA.dirY[p2];
        modeA.radialAccel[p1] = modeA.radialAccel[p2];
        modeA.tangentialAccel[p1] = modeA.tangentialAccel[p2];

        modeB.angle[p1] = modeB.angle[p2];
        modeB.degreesPerSecond[p1] = modeB.degreesPerSecond[p2];
        modeB.radius[p1] = modeB.radius[p2];
        modeB.deltaRadius[p1] = modeB.deltaRadius[p2];

    }
};

struct ParticleVertex {
    Vec2 pos;
    Color4F color;
    float rotation;
    float size;
};

class GPUProgram : public GLProgram {
public:

    void bindPredefinedVertexAttribs();
};

class GPUParticleNode : public Node {
public:
    enum class Mode
    {
        GRAVITY,
        RADIUS,
    };

    enum class PositionType
    {
        FREE, /** Living particles are attached to the world and are unaffected by emitter repositioning. */

        RELATIVE, /** Living particles are attached to the world but will follow the emitter repositioning.
                   Use case: Attach an emitter to an sprite, and you want that the emitter follows the sprite.*/

        GROUPED, /** Living particles are attached to the emitter and are translated along with it. */

    };

    enum {
        /** The Particle emitter lives forever. */
        DURATION_INFINITY = -1,

        /** The starting size of the particle is equal to the ending size. */
        START_SIZE_EQUAL_TO_END_SIZE = -1,

        /** The starting radius of the particle is equal to the ending radius. */
        START_RADIUS_EQUAL_TO_END_RADIUS = -1,
    };

    GPUParticleNode();
    ~GPUParticleNode();

    static GPUParticleNode* createWithFile(const std::string& file);

public:
    bool initWithDictionary(ValueMap& dictionary, const std::string& dirname);
    bool initWithDictionary(ValueMap& dictionary);
    bool initWithFile(const std::string& plistFile);

    virtual bool initWithTotalParticles(int numberOfParticles);

    void update(float dt) override;
    void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;
    void onDraw(const Mat4 &transform, uint32_t flags);


    void updateVertices(float dt);

private:
    void addParticles(int count);
    void emit(float dt);
    
protected:

    std::string _plistFile;

    // properties read from plist file.
    std::string _configName;

    bool _isBlendAdditive;

    /** whether or not the node will be auto-removed when it has no particles left.
     By default it is false.
     @since v0.8
     */
    bool _isAutoRemoveOnFinish;

    //! time elapsed since the start of the system (in seconds)
    float _elapsed;

    // Different modes
    //! Mode A:Gravity + Tangential Accel + Radial Accel
    struct {
        /** Gravity value. Only available in 'Gravity' mode. */
        Vec2 gravity;
        /** speed of each particle. Only available in 'Gravity' mode.  */
        float speed;
        /** speed variance of each particle. Only available in 'Gravity' mode. */
        float speedVar;
        /** tangential acceleration of each particle. Only available in 'Gravity' mode. */
        float tangentialAccel;
        /** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
        float tangentialAccelVar;
        /** radial acceleration of each particle. Only available in 'Gravity' mode. */
        float radialAccel;
        /** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
        float radialAccelVar;
        /** set the rotation of each particle to its direction Only available in 'Gravity' mode. */
        bool rotationIsDir;
    } modeA;

    //! Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
    struct {
        /** The starting radius of the particles. Only available in 'Radius' mode. */
        float startRadius;
        /** The starting radius variance of the particles. Only available in 'Radius' mode. */
        float startRadiusVar;
        /** The ending radius of the particles. Only available in 'Radius' mode. */
        float endRadius;
        /** The ending radius variance of the particles. Only available in 'Radius' mode. */
        float endRadiusVar;
        /** Number of degrees to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
        float rotatePerSecond;
        /** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
        float rotatePerSecondVar;
    } modeB;


    // color modulate
    //    BOOL colorModulate;

    //! How many particles can be emitted per second
    float _emitCounter;

    // Optimization
    //CC_UPDATE_PARTICLE_IMP    updateParticleImp;
    //SEL                        updateParticleSel;

    /** weak reference to the SpriteBatchNode that renders the Sprite */
    ParticleBatchNode* _batchNode;

    // index of system in batch node array
    int _atlasIndex;

    //true if scaled or rotated
    bool _transformSystemDirty;
    // Number of allocated particles
    int _allocatedParticles;

    /** Is the emitter active */
    bool _isActive;

    /** Quantity of particles that are being simulated at the moment */
    int _particleCount;
    /** How many seconds the emitter will run. -1 means 'forever' */
    float _duration;
    /** sourcePosition of the emitter */
    Vec2 _sourcePosition;
    /** Position variance of the emitter */
    Vec2 _posVar;
    /** life, and life variation of each particle */
    float _life;
    /** life variance of each particle */
    float _lifeVar;
    /** angle and angle variation of each particle */
    float _angle;
    /** angle variance of each particle */
    float _angleVar;

    /** Switch between different kind of emitter modes:
     - kParticleModeGravity: uses gravity, speed, radial and tangential acceleration
     - kParticleModeRadius: uses radius movement + rotation
     */
    Mode _emitterMode;

    /** start size in pixels of each particle */
    float _startSize;
    /** size variance in pixels of each particle */
    float _startSizeVar;
    /** end size in pixels of each particle */
    float _endSize;
    /** end size variance in pixels of each particle */
    float _endSizeVar;
    /** start color of each particle */
    Color4F _startColor;
    /** start color variance of each particle */
    Color4F _startColorVar;
    /** end color and end color variation of each particle */
    Color4F _endColor;
    /** end color variance of each particle */
    Color4F _endColorVar;
    //* initial angle of each particle
    float _startSpin;
    //* initial angle of each particle
    float _startSpinVar;
    //* initial angle of each particle
    float _endSpin;
    //* initial angle of each particle
    float _endSpinVar;
    /** emission rate of the particles */
    float _emissionRate;
    /** maximum particles of the system */
    int _totalParticles;
    /** conforms to CocosNodeTexture protocol */
    Texture2D* _texture;
    /** conforms to CocosNodeTexture protocol */
    BlendFunc _blendFunc;
    /** does the alpha value modify color */
    bool _opacityModifyRGB;
    /** does FlippedY variance of each particle */
    int _yCoordFlipped;


    /** particles movement type: Free or Grouped
     @since v0.8
     */
    PositionType _positionType;
    
    /** is the emitter paused */
    bool _paused;

    ParticleVertex   _particleVertices[MAX_VERTICLES];
    GPUParticleData  _particleData;

    GLProgram*      _program;
    VertexBuffer*   _vertexBuffer;

    CustomCommand _customCommand;
};

#endif /* GPUParticleNode_h */
