
uniform sampler2D Texture0;

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main() {
	vec4 color = texture2D(Texture0, v_texCoord);
	gl_FragColor = color * v_fragmentColor;
}