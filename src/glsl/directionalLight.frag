//#version 150

#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
in vec2      fragTexCoord;
in vec3      fragNormal;
in vec3      fragVert;

#define TEXTURE		texture

#else
varying vec2      fragTexCoord;
varying vec3      fragNormal;
varying vec3      fragVert;

#define TEXTURE		texture2D
#endif

uniform mat4 modelMat;
uniform sampler2D tex;
uniform int hasTex;

uniform struct AmbientLight
{
	vec3 intensities;
} ambientLight;

uniform struct DirectionalLight
{
	vec3 direction;
	vec3 intensities;
} directionalLight;

uniform struct Material
{
    vec3 ka;
    vec3 kd;
} material;

void main()
{
    vec4 surfaceColor = TEXTURE(tex, fragTexCoord);
    if(hasTex == 0) surfaceColor = vec4(1,1,1,1);
    
 
    
	gl_FragColor = vec4(surfaceColor.rgb,1.0);
//	gl_FragColor = vec4(fragNormal,1);
//	gl_FragColor = vec4(fragTexCoord,0.5,1);
}
