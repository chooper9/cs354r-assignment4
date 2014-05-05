attribute vec4 position;
attribute vec3 normal;
attribute vec2 uv0;

varying float intensity;
varying vec2 vTexCoord;
varying vec4 Ka;
varying vec4 modCol;

uniform mat4 worldViewProj;
uniform vec4 lpos;
uniform vec4 ambient;
uniform vec4 inCol;

void main()                    
{
	vec3 n = normalize(normal);
	vec3 ldir = normalize(lpos.xyz - (position.xyz*lpos.w));
	intensity = max(dot(ldir, n), 0.0);

	gl_Position = worldViewProj * position;
	vTexCoord = uv0;
	Ka = ambient;
	modCol = inCol;
}
