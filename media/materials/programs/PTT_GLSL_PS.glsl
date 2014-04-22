varying vec2 vTexCoord;
varying vec4 modCol;

uniform sampler2D myTexture;

void main()
{
	gl_FragColor = texture2D(myTexture, vTexCoord) * modCol;
}
