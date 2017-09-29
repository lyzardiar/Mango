//
//  Created by yiyang.tang on 2017/9/4.
//
#include "GPUParticleNode.h"

const char* vs_particle = R"(
#ifdef GL_ES
precision lowp float;
#endif

attribute vec2  in_position;
attribute vec4  in_color;
attribute float in_rotation;
attribute float in_size;

varying vec4 v_fragmentColor;
varying float v_rotation;

void main()
{
    gl_Position = CC_MVPMatrix * vec4(in_position, 1.0, 1.0);
    gl_PointSize = in_size;
    v_fragmentColor = in_color;
    v_rotation = in_rotation;
}

)";

const char* fs_particle = R"(
#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying float v_rotation;

const float mid = 0.5;
void main()
{
    vec2 rotatedCoord = vec2(cos(v_rotation) * (gl_PointCoord.x - mid) + sin(v_rotation) * (gl_PointCoord.y - mid) + mid,
                        cos(v_rotation) * (gl_PointCoord.y - mid) - sin(v_rotation) * (gl_PointCoord.x - mid) + mid);

    gl_FragColor = texture2D(CC_Texture0, rotatedCoord) * v_fragmentColor;
}
)";

inline void normalize_point(float x, float y, particle_point* out)
{
    float n = x * x + y * y;
    // Already normalized.
    if (n == 1.0f || n < MATH_TOLERANCE)
        return;

    n = sqrt(n);

    n = 1.0f / n;
    out->x = x * n;
    out->y = y * n;
}

inline static float RANDOM_M11(unsigned int *seed) {
    *seed = *seed * 134775813 + 1;
    union {
        uint32_t d;
        float f;
    } u;
    u.d = (((uint32_t)(*seed) & 0x7fff) << 8) | 0x40000000;
    return u.f - 3.0f;
}

void GPUProgram::bindPredefinedVertexAttribs()
{
    static const struct {
        const char *attributeName;
        int location;
    } attribute_locations[] =
    {
        {"in_position",    0},
        {"in_color",       1},
        {"in_rotation",    2},
        {"in_size",        3},
    };

    const int size = sizeof(attribute_locations) / sizeof(attribute_locations[0]);

    log("n attribute = %d", size);
    for(int i=0; i<size;i++) {
        glEnableVertexAttribArray(i);
        glBindAttribLocation(_program, attribute_locations[i].location, attribute_locations[i].attributeName);
        CHECK_GL_ERROR_DEBUG();
    }
}


GPUParticleData::GPUParticleData()
{
    memset(this, 0, sizeof(ParticleData));
}

bool GPUParticleData::init(int count)
{
    maxCount = count;

    posx= (float*)malloc(count * sizeof(float));
    posy= (float*)malloc(count * sizeof(float));
    startPosX= (float*)malloc(count * sizeof(float));
    startPosY= (float*)malloc(count * sizeof(float));
    colorR= (float*)malloc(count * sizeof(float));
    colorG= (float*)malloc(count * sizeof(float));
    colorB= (float*)malloc(count * sizeof(float));
    colorA= (float*)malloc(count * sizeof(float));
    deltaColorR= (float*)malloc(count * sizeof(float));
    deltaColorG= (float*)malloc(count * sizeof(float));
    deltaColorB= (float*)malloc(count * sizeof(float));
    deltaColorA= (float*)malloc(count * sizeof(float));
    size= (float*)malloc(count * sizeof(float));
    deltaSize= (float*)malloc(count * sizeof(float));
    rotation= (float*)malloc(count * sizeof(float));
    deltaRotation= (float*)malloc(count * sizeof(float));
    timeToLive= (float*)malloc(count * sizeof(float));
    atlasIndex= (unsigned int*)malloc(count * sizeof(unsigned int));

    modeA.dirX= (float*)malloc(count * sizeof(float));
    modeA.dirY= (float*)malloc(count * sizeof(float));
    modeA.radialAccel= (float*)malloc(count * sizeof(float));
    modeA.tangentialAccel= (float*)malloc(count * sizeof(float));

    modeB.angle= (float*)malloc(count * sizeof(float));
    modeB.degreesPerSecond= (float*)malloc(count * sizeof(float));
    modeB.deltaRadius= (float*)malloc(count * sizeof(float));
    modeB.radius= (float*)malloc(count * sizeof(float));

    return posx && posy && startPosY && startPosX && colorR && colorG && colorB && colorA &&
    deltaColorR && deltaColorG && deltaColorB && deltaColorA && size && deltaSize &&
    rotation && deltaRotation && timeToLive && atlasIndex && modeA.dirX && modeA.dirY &&
    modeA.radialAccel && modeA.tangentialAccel && modeB.angle && modeB.degreesPerSecond &&
    modeB.deltaRadius && modeB.radius;
}

void GPUParticleData::release()
{
    CC_SAFE_FREE(posx);
    CC_SAFE_FREE(posy);
    CC_SAFE_FREE(startPosX);
    CC_SAFE_FREE(startPosY);
    CC_SAFE_FREE(colorR);
    CC_SAFE_FREE(colorG);
    CC_SAFE_FREE(colorB);
    CC_SAFE_FREE(colorA);
    CC_SAFE_FREE(deltaColorR);
    CC_SAFE_FREE(deltaColorG);
    CC_SAFE_FREE(deltaColorB);
    CC_SAFE_FREE(deltaColorA);
    CC_SAFE_FREE(size);
    CC_SAFE_FREE(deltaSize);
    CC_SAFE_FREE(rotation);
    CC_SAFE_FREE(deltaRotation);
    CC_SAFE_FREE(timeToLive);
    CC_SAFE_FREE(atlasIndex);

    CC_SAFE_FREE(modeA.dirX);
    CC_SAFE_FREE(modeA.dirY);
    CC_SAFE_FREE(modeA.radialAccel);
    CC_SAFE_FREE(modeA.tangentialAccel);

    CC_SAFE_FREE(modeB.angle);
    CC_SAFE_FREE(modeB.degreesPerSecond);
    CC_SAFE_FREE(modeB.deltaRadius);
    CC_SAFE_FREE(modeB.radius);
}



GPUParticleNode::GPUParticleNode()
{

}

GPUParticleNode::~GPUParticleNode()
{
    CC_SAFE_RELEASE(_program);
    CC_SAFE_RELEASE(_vertexBuffer);

}

GPUParticleNode* GPUParticleNode::createWithFile(const std::string& file)
{
    GPUParticleNode* node = new GPUParticleNode();
    if (node && node->initWithFile(file)) {
        node->autorelease();
        return node;
    } else {
        CC_SAFE_DELETE(node);
        return nullptr;
    }
}

bool GPUParticleNode::initWithFile(const std::string& plistFile)
{
    bool ret = false;
    _plistFile = FileUtils::getInstance()->fullPathForFilename(plistFile);
    ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(_plistFile);

    CCASSERT( !dict.empty(), "Particles: file not found");

    // FIXME: compute path from a path, should define a function somewhere to do it
    std::string listFilePath = plistFile;
    if (listFilePath.find('/') != std::string::npos)
    {
        listFilePath = listFilePath.substr(0, listFilePath.rfind('/') + 1);
        ret = this->initWithDictionary(dict, listFilePath);
    }
    else
    {
        ret = this->initWithDictionary(dict, "");
    }

    return ret;
}

bool GPUParticleNode::initWithDictionary(ValueMap& dictionary)
{
    return initWithDictionary(dictionary, "");
}

bool GPUParticleNode::initWithDictionary(ValueMap& dictionary, const std::string& dirname)
{
    // TODO: implement this in my own way. This code takes from CCParticleSystem, and could be better.
    bool ret = false;
    unsigned char *buffer = nullptr;
    unsigned char *deflated = nullptr;
    Image *image = nullptr;
    do
    {
        int maxParticles = dictionary["maxParticles"].asInt();
//        int maxParticles = 100;
        // self, not super
        if(this->initWithTotalParticles(maxParticles))
        {
            // Emitter name in particle designer 2.0
            _configName = dictionary["configName"].asString();

            // angle
            _angle = dictionary["angle"].asFloat();
            _angleVar = dictionary["angleVariance"].asFloat();

            // duration
            _duration = dictionary["duration"].asFloat();

            // blend function

            _blendFunc.src = dictionary["blendFuncSource"].asInt();
            _blendFunc.dst = dictionary["blendFuncDestination"].asInt();

            // color
            _startColor.r = dictionary["startColorRed"].asFloat();
            _startColor.g = dictionary["startColorGreen"].asFloat();
            _startColor.b = dictionary["startColorBlue"].asFloat();
            _startColor.a = dictionary["startColorAlpha"].asFloat();

            _startColorVar.r = dictionary["startColorVarianceRed"].asFloat();
            _startColorVar.g = dictionary["startColorVarianceGreen"].asFloat();
            _startColorVar.b = dictionary["startColorVarianceBlue"].asFloat();
            _startColorVar.a = dictionary["startColorVarianceAlpha"].asFloat();

            _endColor.r = dictionary["finishColorRed"].asFloat();
            _endColor.g = dictionary["finishColorGreen"].asFloat();
            _endColor.b = dictionary["finishColorBlue"].asFloat();
            _endColor.a = dictionary["finishColorAlpha"].asFloat();

            _endColorVar.r = dictionary["finishColorVarianceRed"].asFloat();
            _endColorVar.g = dictionary["finishColorVarianceGreen"].asFloat();
            _endColorVar.b = dictionary["finishColorVarianceBlue"].asFloat();
            _endColorVar.a = dictionary["finishColorVarianceAlpha"].asFloat();

            // particle size
            _startSize = dictionary["startParticleSize"].asFloat();
            _startSizeVar = dictionary["startParticleSizeVariance"].asFloat();
            _endSize = dictionary["finishParticleSize"].asFloat();
            _endSizeVar = dictionary["finishParticleSizeVariance"].asFloat();

            // position
            float x = dictionary["sourcePositionx"].asFloat();
            float y = dictionary["sourcePositiony"].asFloat();
            this->setPosition(x,y);
            _posVar.x = dictionary["sourcePositionVariancex"].asFloat();
            _posVar.y = dictionary["sourcePositionVariancey"].asFloat();

            // Spinning
            _startSpin = dictionary["rotationStart"].asFloat();
            _startSpinVar = dictionary["rotationStartVariance"].asFloat();
            _endSpin= dictionary["rotationEnd"].asFloat();
            _endSpinVar= dictionary["rotationEndVariance"].asFloat();

            _emitterMode = (Mode)dictionary["emitterType"].asInt();

            // Mode A: Gravity + tangential accel + radial accel
            if (_emitterMode == Mode::GRAVITY)
            {
                // gravity
                modeA.gravity.x = dictionary["gravityx"].asFloat();
                modeA.gravity.y = dictionary["gravityy"].asFloat();

                // speed
                modeA.speed = dictionary["speed"].asFloat();
                modeA.speedVar = dictionary["speedVariance"].asFloat();

                // radial acceleration
                modeA.radialAccel = dictionary["radialAcceleration"].asFloat();
                modeA.radialAccelVar = dictionary["radialAccelVariance"].asFloat();

                // tangential acceleration
                modeA.tangentialAccel = dictionary["tangentialAcceleration"].asFloat();
                modeA.tangentialAccelVar = dictionary["tangentialAccelVariance"].asFloat();

                // rotation is dir
                modeA.rotationIsDir = dictionary["rotationIsDir"].asBool();
            }

            // or Mode B: radius movement
            else if (_emitterMode == Mode::RADIUS)
            {
                if (!_configName.empty())
                {
                    modeB.startRadius = dictionary["maxRadius"].asInt();
                }
                else
                {
                    modeB.startRadius = dictionary["maxRadius"].asFloat();
                }
                modeB.startRadiusVar = dictionary["maxRadiusVariance"].asFloat();
                if (!_configName.empty())
                {
                    modeB.endRadius = dictionary["minRadius"].asInt();
                }
                else
                {
                    modeB.endRadius = dictionary["minRadius"].asFloat();
                }

                if (dictionary.find("minRadiusVariance") != dictionary.end())
                {
                    modeB.endRadiusVar = dictionary["minRadiusVariance"].asFloat();
                }
                else
                {
                    modeB.endRadiusVar = 0.0f;
                }

                if (!_configName.empty())
                {
                    modeB.rotatePerSecond = dictionary["rotatePerSecond"].asInt();
                }
                else
                {
                    modeB.rotatePerSecond = dictionary["rotatePerSecond"].asFloat();
                }
                modeB.rotatePerSecondVar = dictionary["rotatePerSecondVariance"].asFloat();

            } else {
                CCASSERT( false, "Invalid emitterType in config file");
                CC_BREAK_IF(true);
            }

            // life span
            _life = dictionary["particleLifespan"].asFloat();
            _lifeVar = dictionary["particleLifespanVariance"].asFloat();

            // emission Rate
            _emissionRate = _totalParticles / _life;

            // texture
            // Try to get the texture from the cache
            std::string textureName = dictionary["textureFileName"].asString();

            size_t rPos = textureName.rfind('/');

            if (rPos != std::string::npos)
            {
                std::string textureDir = textureName.substr(0, rPos + 1);

                if (!dirname.empty() && textureDir != dirname)
                {
                    textureName = textureName.substr(rPos+1);
                    textureName = dirname + textureName;
                }
            }
            else if (!dirname.empty() && !textureName.empty())
            {
                textureName = dirname + textureName;
            }

            Texture2D *tex = nullptr;

            if (!textureName.empty())
            {
                // set not pop-up message box when load image failed
                bool notify = FileUtils::getInstance()->isPopupNotify();
                FileUtils::getInstance()->setPopupNotify(false);
                tex = Director::getInstance()->getTextureCache()->addImage(textureName);
                // reset the value of UIImage notify
                FileUtils::getInstance()->setPopupNotify(notify);
            }

            if (tex)
            {
                _texture = tex;
            }
            else if( dictionary.find("textureImageData") != dictionary.end() )
            {
                std::string textureData = dictionary.at("textureImageData").asString();
                CCASSERT(!textureData.empty(), "textureData can't be empty!");

                auto dataLen = textureData.size();
                if (dataLen != 0)
                {
                    // if it fails, try to get it from the base64-gzipped data
                    int decodeLen = base64Decode((unsigned char*)textureData.c_str(), (unsigned int)dataLen, &buffer);
                    CCASSERT( buffer != nullptr, "CCParticleSystem: error decoding textureImageData");
                    CC_BREAK_IF(!buffer);

                    ssize_t deflatedLen = ZipUtils::inflateMemory(buffer, decodeLen, &deflated);
                    CCASSERT( deflated != nullptr, "CCParticleSystem: error ungzipping textureImageData");
                    CC_BREAK_IF(!deflated);

                    // For android, we should retain it in VolatileTexture::addImage which invoked in Director::getInstance()->getTextureCache()->addUIImage()
                    image = new (std::nothrow) Image();
                    bool isOK = image->initWithImageData(deflated, deflatedLen);
                    CCASSERT(isOK, "CCParticleSystem: error init image with Data");
                    CC_BREAK_IF(!isOK);

                    _texture = Director::getInstance()->getTextureCache()->addImage(image, _plistFile + textureName);
                    image->release();
                }
            }

            _yCoordFlipped = dictionary.find("yCoordFlipped") == dictionary.end() ? 1 : dictionary.at("yCoordFlipped").asInt();
            
            if( !this->_texture)
                CCLOGWARN("cocos2d: Warning: ParticleSystemQuad system without a texture");

            ret = true;
        }
    } while (0);

    free(buffer);
    free(deflated);
    return ret;
}


bool GPUParticleNode::initWithTotalParticles(int numberOfParticles)
{
    _totalParticles = numberOfParticles;
    _isActive = true;
    _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
    _emitterMode = Mode::GRAVITY;

    _program = new (std::nothrow) GPUProgram();
    if(_program && _program->initWithByteArrays(vs_particle, fs_particle)) {
        _program->link();
        _program->updateUniforms();
    }
    _program->retain();

    GLuint loc_pos = glGetAttribLocation(_program->getProgram(), "in_position");
    GLuint loc_color = glGetAttribLocation(_program->getProgram(), "in_color");
    GLuint loc_size =  glGetAttribLocation(_program->getProgram(), "in_size");
    GLuint loc_rotation = glGetAttribLocation(_program->getProgram(), "in_rotation");

    log("location = %d, %d, %d, %d", loc_pos, loc_color, loc_size, loc_rotation);

    _vertexBuffer = VertexBuffer::create(sizeof(ParticleVertex), _totalParticles, GL_DYNAMIC_DRAW);
    CC_SAFE_RETAIN(_vertexBuffer);

    _particleData.release();
    _particleData.init(_totalParticles);
    
    _particleCount = 0;
    _emitCounter = 0.0f;
    
    this->scheduleUpdate();

    return true;
}

void GPUParticleNode::update(float dt)
{
    this->emit(dt);
    this->updateVertices(dt);
}

void GPUParticleNode::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(GPUParticleNode::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void GPUParticleNode::onDraw(const Mat4 &transform, uint32_t flags)
{

    _program->use();
    CHECK_GL_ERROR_DEBUG();
    _program->setUniformsForBuiltins(transform);
    CHECK_GL_ERROR_DEBUG();
    //set blend mode
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    GL::bindTexture2D(_texture);

    CHECK_GL_ERROR_DEBUG();

    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer->getVBO());

    glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, pos));
    glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, color));
    glVertexAttribPointer(2, 1, GL_FLOAT, false, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, rotation));
    glVertexAttribPointer(3, 1, GL_FLOAT, false, sizeof(ParticleVertex), (void*)offsetof(ParticleVertex, size));



    CHECK_GL_ERROR_DEBUG();
    glDrawArrays(GL_POINTS, 0, _particleCount);

    CHECK_GL_ERROR_DEBUG();
}

void GPUParticleNode::updateVertices(float dt)
{
    for (int i = 0; i < _particleCount; ++i) {

        _particleVertices[i].pos.x = _particleData.posx[i];
        _particleVertices[i].pos.y = _particleData.posy[i];

        _particleVertices[i].color.r = _particleData.colorR[i];
        _particleVertices[i].color.g = _particleData.colorG[i];
        _particleVertices[i].color.b = _particleData.colorB[i];
        _particleVertices[i].color.a = _particleData.colorA[i];


        _particleVertices[i].size = _particleData.size[i];

        _particleVertices[i].rotation =  CC_DEGREES_TO_RADIANS(_particleData.rotation[i]);


        log("_particleVertices rotation[%d] = %.2f", i, _particleVertices[i].rotation);
//        log("_particleVertices[%d].xy = %.2f, %.2f", i, _particleVertices[i].pos.x, _particleVertices[i].pos.y);

    }

    _vertexBuffer->updateVertices(_particleVertices, _particleCount, 0);
}

void GPUParticleNode::addParticles(int count)
{
    if (_paused)
        return;
    uint32_t RANDSEED = rand();

    int start = _particleCount;
    _particleCount += count;

    //life
    for (int i = start; i < _particleCount ; ++i)
    {
        float theLife = _life + _lifeVar * RANDOM_M11(&RANDSEED);
        _particleData.timeToLive[i] = MAX(0, theLife);
    }

    //position
    for (int i = start; i < _particleCount; ++i)
    {
        _particleData.posx[i] = _sourcePosition.x + _posVar.x * RANDOM_M11(&RANDSEED);
    }

    for (int i = start; i < _particleCount; ++i)
    {
        _particleData.posy[i] = _sourcePosition.y + _posVar.y * RANDOM_M11(&RANDSEED);
    }

    //color
#define SET_COLOR(c, b, v)\
for (int i = start; i < _particleCount; ++i)\
{\
c[i] = clampf( b + v * RANDOM_M11(&RANDSEED) , 0 , 1 );\
}

    SET_COLOR(_particleData.colorR, _startColor.r, _startColorVar.r);
    SET_COLOR(_particleData.colorG, _startColor.g, _startColorVar.g);
    SET_COLOR(_particleData.colorB, _startColor.b, _startColorVar.b);
    SET_COLOR(_particleData.colorA, _startColor.a, _startColorVar.a);

    SET_COLOR(_particleData.deltaColorR, _endColor.r, _endColorVar.r);
    SET_COLOR(_particleData.deltaColorG, _endColor.g, _endColorVar.g);
    SET_COLOR(_particleData.deltaColorB, _endColor.b, _endColorVar.b);
    SET_COLOR(_particleData.deltaColorA, _endColor.a, _endColorVar.a);

#define SET_DELTA_COLOR(c, dc)\
for (int i = start; i < _particleCount; ++i)\
{\
dc[i] = (dc[i] - c[i]) / _particleData.timeToLive[i];\
}

    SET_DELTA_COLOR(_particleData.colorR, _particleData.deltaColorR);
    SET_DELTA_COLOR(_particleData.colorG, _particleData.deltaColorG);
    SET_DELTA_COLOR(_particleData.colorB, _particleData.deltaColorB);
    SET_DELTA_COLOR(_particleData.colorA, _particleData.deltaColorA);

    //size
    for (int i = start; i < _particleCount; ++i)
    {
        _particleData.size[i] = _startSize + _startSizeVar * RANDOM_M11(&RANDSEED);
        _particleData.size[i] = MAX(0, _particleData.size[i]);
    }

    if (_endSize != START_SIZE_EQUAL_TO_END_SIZE)
    {
        for (int i = start; i < _particleCount; ++i)
        {
            float endSize = _endSize + _endSizeVar * RANDOM_M11(&RANDSEED);
            endSize = MAX(0, endSize);
            _particleData.deltaSize[i] = (endSize - _particleData.size[i]) / _particleData.timeToLive[i];
        }
    }
    else
    {
        for (int i = start; i < _particleCount; ++i)
        {
            _particleData.deltaSize[i] = 0.0f;
        }
    }

    // rotation
    for (int i = start; i < _particleCount; ++i)
    {
        _particleData.rotation[i] = _startSpin + _startSpinVar * RANDOM_M11(&RANDSEED);
    }
    for (int i = start; i < _particleCount; ++i)
    {
        float endA = _endSpin + _endSpinVar * RANDOM_M11(&RANDSEED);
        _particleData.deltaRotation[i] = (endA - _particleData.rotation[i]) / _particleData.timeToLive[i];
    }

    // position
    Vec2 pos;
    if (_positionType == PositionType::FREE)
    {
        pos = this->convertToWorldSpace(Vec2::ZERO);
    }
    else if (_positionType == PositionType::RELATIVE)
    {
        pos = _position;
    }
    for (int i = start; i < _particleCount; ++i)
    {
        _particleData.startPosX[i] = pos.x;
    }
    for (int i = start; i < _particleCount; ++i)
    {
        _particleData.startPosY[i] = pos.y;
    }

    // Mode Gravity: A
    if (_emitterMode == Mode::GRAVITY)
    {

        // radial accel
        for (int i = start; i < _particleCount; ++i)
        {
            _particleData.modeA.radialAccel[i] = modeA.radialAccel + modeA.radialAccelVar * RANDOM_M11(&RANDSEED);
        }

        // tangential accel
        for (int i = start; i < _particleCount; ++i)
        {
            _particleData.modeA.tangentialAccel[i] = modeA.tangentialAccel + modeA.tangentialAccelVar * RANDOM_M11(&RANDSEED);
        }

        // rotation is dir
        if( modeA.rotationIsDir )
        {
            for (int i = start; i < _particleCount; ++i)
            {
                float a = CC_DEGREES_TO_RADIANS( _angle + _angleVar * RANDOM_M11(&RANDSEED) );
                Vec2 v(cosf( a ), sinf( a ));
                float s = modeA.speed + modeA.speedVar * RANDOM_M11(&RANDSEED);
                Vec2 dir = v * s;
                _particleData.modeA.dirX[i] = dir.x;//v * s ;
                _particleData.modeA.dirY[i] = dir.y;
                _particleData.rotation[i] = -CC_RADIANS_TO_DEGREES(dir.getAngle());
            }
        }
        else
        {
            for (int i = start; i < _particleCount; ++i)
            {
                float a = CC_DEGREES_TO_RADIANS( _angle + _angleVar * RANDOM_M11(&RANDSEED) );
                Vec2 v(cosf( a ), sinf( a ));
                float s = modeA.speed + modeA.speedVar * RANDOM_M11(&RANDSEED);
                Vec2 dir = v * s;
                _particleData.modeA.dirX[i] = dir.x;//v * s ;
                _particleData.modeA.dirY[i] = dir.y;
            }
        }

    }

    // Mode Radius: B
    else
    {
        //Need to check by Jacky
        // Set the default diameter of the particle from the source position
        for (int i = start; i < _particleCount; ++i)
        {
            _particleData.modeB.radius[i] = modeB.startRadius + modeB.startRadiusVar * RANDOM_M11(&RANDSEED);
        }

        for (int i = start; i < _particleCount; ++i)
        {
            _particleData.modeB.angle[i] = CC_DEGREES_TO_RADIANS( _angle + _angleVar * RANDOM_M11(&RANDSEED));
        }

        for (int i = start; i < _particleCount; ++i)
        {
            _particleData.modeB.degreesPerSecond[i] = CC_DEGREES_TO_RADIANS(modeB.rotatePerSecond + modeB.rotatePerSecondVar * RANDOM_M11(&RANDSEED));
        }

        if(modeB.endRadius == -1)
        {
            for (int i = start; i < _particleCount; ++i)
            {
                _particleData.modeB.deltaRadius[i] = 0.0f;
            }
        }
        else
        {
            for (int i = start; i < _particleCount; ++i)
            {
                float endRadius = modeB.endRadius + modeB.endRadiusVar * RANDOM_M11(&RANDSEED);
                _particleData.modeB.deltaRadius[i] = (endRadius - _particleData.modeB.radius[i]) / _particleData.timeToLive[i];
            }
        }
    }
    
}

void GPUParticleNode::emit(float dt)
{
    float rate = 1.0f / _emissionRate;
    //issue #1201, prevent bursts of particles, due to too high emitCounter
    if (_particleCount < _totalParticles)
    {
        _emitCounter += dt;
        if (_emitCounter < 0.f)
            _emitCounter = 0.f;
    }

    int emitCount = MIN(_totalParticles - _particleCount, _emitCounter / rate);
    addParticles(emitCount);
    _emitCounter -= rate * emitCount;


    for (int i = 0; i < _particleCount; ++i)
    {
        _particleData.timeToLive[i] -= dt;
    }

    for (int i = 0; i < _particleCount; ++i)
    {
        if (_particleData.timeToLive[i] <= 0.0f)
        {
            int j = _particleCount - 1;
            while (j > 0 && _particleData.timeToLive[j] <= 0)
            {
                _particleCount--;
                j--;
            }
            _particleData.copyParticle(i, _particleCount - 1);
            --_particleCount;
        }
    }

    if (_emitterMode == Mode::GRAVITY)
    {
        for (int i = 0 ; i < _particleCount; ++i)
        {
            particle_point tmp, radial = {0.0f, 0.0f}, tangential;

            // radial acceleration
            if (_particleData.posx[i] || _particleData.posy[i])
            {
                normalize_point(_particleData.posx[i], _particleData.posy[i], &radial);
            }
            tangential = radial;
            radial.x *= _particleData.modeA.radialAccel[i];
            radial.y *= _particleData.modeA.radialAccel[i];

            // tangential acceleration
            std::swap(tangential.x, tangential.y);
            tangential.x *= - _particleData.modeA.tangentialAccel[i];
            tangential.y *= _particleData.modeA.tangentialAccel[i];

            // (gravity + radial + tangential) * dt
            tmp.x = radial.x + tangential.x + modeA.gravity.x;
            tmp.y = radial.y + tangential.y + modeA.gravity.y;
            tmp.x *= dt;
            tmp.y *= dt;

            _particleData.modeA.dirX[i] += tmp.x;
            _particleData.modeA.dirY[i] += tmp.y;

            // this is cocos2d-x v3.0
            // if (_configName.length()>0 && _yCoordFlipped != -1)

            // this is cocos2d-x v3.0
            tmp.x = _particleData.modeA.dirX[i] * dt * _yCoordFlipped;
            tmp.y = _particleData.modeA.dirY[i] * dt * _yCoordFlipped;
            _particleData.posx[i] += tmp.x;
            _particleData.posy[i] += tmp.y;
        }
    }
    else
    {
        //Why use so many for-loop separately instead of putting them together?
        //When the processor needs to read from or write to a location in memory,
        //it first checks whether a copy of that data is in the cache.
        //And every property's memory of the particle system is continuous,
        //for the purpose of improving cache hit rate, we should process only one property in one for-loop AFAP.
        //It was proved to be effective especially for low-end machine.
        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.modeB.angle[i] += _particleData.modeB.degreesPerSecond[i] * dt;
        }

        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.modeB.radius[i] += _particleData.modeB.deltaRadius[i] * dt;
        }

        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.posx[i] = - cosf(_particleData.modeB.angle[i]) * _particleData.modeB.radius[i];
        }
        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.posy[i] = - sinf(_particleData.modeB.angle[i]) * _particleData.modeB.radius[i] * _yCoordFlipped;
        }
    }

    //color r,g,b,a
    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particleData.colorR[i] += _particleData.deltaColorR[i] * dt;
    }

    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particleData.colorG[i] += _particleData.deltaColorG[i] * dt;
    }

    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particleData.colorB[i] += _particleData.deltaColorB[i] * dt;
    }

    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particleData.colorA[i] += _particleData.deltaColorA[i] * dt;
    }
    //size
    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particleData.size[i] += (_particleData.deltaSize[i] * dt);
        _particleData.size[i] = MAX(0, _particleData.size[i]);
    }
    //angle
    for (int i = 0 ; i < _particleCount; ++i)
    {
        _particleData.rotation[i] += _particleData.deltaRotation[i] * dt;
    }

//    for (int i = 0; i < _particleCount; ++i)
//    {
//        log("_particleData[%d].timeToLive = %.2f", i, _particleData.timeToLive[i]);
//        log("_particleData[%d].posx = %.2f", i, _particleData.posx[i]);
//        log("_particleData[%d].posy = %.2f", i, _particleData.posy[i]);
//        log("_particleData[%d].color = %.2f, %.2f, %.2f, %.2f", i,  _particleData.colorR[i],
//                                                                    _particleData.colorG[i],
//                                                                    _particleData.colorB[i],
//                                                                    _particleData.colorA[i]);
//
//
//    }


}
