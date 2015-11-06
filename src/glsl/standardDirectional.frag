//#version 150

#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
in vec2      fragTexCoord;
in vec3      fragNormal;
in vec3      fragVert;
#else
varying vec2      fragTexCoord;
varying vec3      fragNormal;
varying vec3      fragVert;
#endif

uniform mat4 modelMat;
uniform sampler2D tex;
uniform int hasTex;

uniform struct AmbientLight
{
	vec3 intensities;//°´ÑÕÉ«µÄÇ¿¶È£¬ÒÑ¾­³Ë¹ýÇ¿¶ÈÏµÊý
}ambientLight;

uniform vec3 ka;//ÎïÌåµÄ»·¾³¹â·´ÉäÏµÊý

uniform struct DirectionalLight
{
	vec3 direction;//²»±Ø¹éÒ»»¯
	vec3 intensities;//°´ÑÕÉ«µÄÇ¿¶È£¬ÒÑ¾­³Ë¹ýÇ¿¶ÈÏµÊý
}directionalLight;//·´ÉäÏµÊýÊ¹ÓÃKd

uniform vec3 kd;//ÎïÌåµÄÉ¢Éä¹â·´ÉäÏµÊý

void main() {
	vec3 lightDirection = normalize(directionalLight.direction);

    //calculate normal in world coordinates
    mat3 normalMatrix = mat3(modelMat);//transpose(inverse(mat3(modelMat)));
    vec3 normal = normalize(normalMatrix * fragNormal);
    
    float brightness = -dot(normal, lightDirection);//normal¡¢lightDirectionÒª¹éÒ»»¯
    brightness = clamp(brightness, 0.0, 1.0);

    //calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: light.intensities
	// 3. The reflection coefficient(ka for Ambient & kd for Diffuse) of the surface
    // 4. The texture and texture coord: texture(tex, fragTexCoord)
	vec4 surfaceColor = texture2D(tex, fragTexCoord);

	if(hasTex == 0) surfaceColor = vec4(1,1,1,1);

	vec3 fragColorRGB = ambientLight.intensities * ka +				//»·¾³¹â
		brightness * directionalLight.intensities * surfaceColor.rgb * kd.rgb;	//Æ½ÐÐ¹â
	
    gl_FragColor = vec4(fragColorRGB, surfaceColor.a);

//	gl_FragColor = vec4(fragNormal,1);
	//gl_FragColor = vec4(fragTexCoord,0.5,1);
}
