varying float intensity;
varying vec2 vTexCoord;
varying vec4 Ka;
varying vec4 modCol;

uniform vec4 lightDiffuse;
uniform sampler2D myTexture;
uniform sampler2D myOtherTexture;

void main()
{
	vec4 Kd = lightDiffuse*intensity;

	vec4 darkerCol = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 texCol = (texture2D(myTexture, vTexCoord)*modCol)
			+ (texture2D(myOtherTexture, vTexCoord)*darkerCol);
	gl_FragColor = (Kd + Ka)*texCol;
	gl_FragColor.a = 1.0;
}
