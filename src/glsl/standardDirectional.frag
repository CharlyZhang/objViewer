#version 150

uniform mat4 modelMat;
uniform sampler2D tex;
uniform int hasTex;

uniform struct AmbientLight
{
	vec3 intensities;//����ɫ��ǿ�ȣ��Ѿ��˹�ǿ��ϵ��
}ambientLight;

uniform vec3 ka;//����Ļ����ⷴ��ϵ��

uniform struct DirectionalLight
{
	vec3 direction;//���ع�һ��
	vec3 intensities;//����ɫ��ǿ�ȣ��Ѿ��˹�ǿ��ϵ��
}directionalLight;//����ϵ��ʹ��Kd

uniform vec3 kd;//�����ɢ��ⷴ��ϵ��

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

void main() {
	vec3 lightDirection = normalize(directionalLight.direction);

    //calculate normal in world coordinates
    mat3 normalMatrix = transpose(inverse(mat3(modelMat)));
    vec3 normal = normalize(normalMatrix * fragNormal);
    
    float brightness = -dot(normal, lightDirection);//normal��lightDirectionҪ��һ��
    brightness = clamp(brightness, 0, 1);

    //calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: light.intensities
	// 3. The reflection coefficient(ka for Ambient & kd for Diffuse) of the surface
    // 4. The texture and texture coord: texture(tex, fragTexCoord)
	vec4 surfaceColor = texture(tex, fragTexCoord);

	if(hasTex == 0) surfaceColor = vec4(1,1,1,1);

	vec3 fragColorRGB = ambientLight.intensities * ka +				//������
		brightness * directionalLight.intensities * surfaceColor.rgb * kd.rgb;	//ƽ�й�
	
    gl_FragColor = vec4(fragColorRGB, surfaceColor.a);

//	gl_FragColor = vec4(fragNormal,1);
	//gl_FragColor = vec4(fragTexCoord,0.5,1);
}