#ifdef GL_ES
precision lowp float;
#endif

uniform sampler2D Texture0;

varying vec4 v_fragmentColor;
varying float v_rotation;

const float mid = 0.5;
void main()
{
    vec2 rotatedCoord = vec2(cos(v_rotation) * (gl_PointCoord.x - mid) + sin(v_rotation) * (gl_PointCoord.y - mid) + mid,
                        cos(v_rotation) * (gl_PointCoord.y - mid) - sin(v_rotation) * (gl_PointCoord.x - mid) + mid);

    gl_FragColor = texture2D(Texture0, rotatedCoord) * v_fragmentColor;
}