attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord;

uniform mat4 MatP;
uniform mat4 MatM;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main()						
{							
	gl_Position = MatP * MatM * a_position;
	gl_Position.w = 1.0;

	v_fragmentColor	= a_color;
	v_texCoord	= a_texCoord;
}	