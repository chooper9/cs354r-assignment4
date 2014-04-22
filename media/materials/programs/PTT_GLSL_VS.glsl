attribute vec4 position;
attribute vec2 uv0;

varying vec2 vTexCoord;
varying vec4 modCol;

uniform mat4 worldViewProj;
uniform vec4 inCol;

void main()                    
{
	gl_Position = worldViewProj * position;
	vTexCoord = uv0;
	modCol = inCol;
}
