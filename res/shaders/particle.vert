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
	gl_Position = MatP * MatM * vec4(in_position, 1.0, 1.0);
	gl_Position.w = 1.0;

    gl_PointSize = in_size;
    v_fragmentColor = in_color;
    v_rotation = in_rotation;
}