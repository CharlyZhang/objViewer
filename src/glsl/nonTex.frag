#version 150

uniform mat4 modelMat;

uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
} light;

uniform vec3 kd;//�����ɢ��ⷴ��ϵ��

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVert;

void main() {
    //calculate normal in world coordinates
    mat3 normalMatrix = transpose(inverse(mat3(modelMat)));
    vec3 normal = normalize(normalMatrix * fragNormal);
    
    //calculate the location of this fragment (pixel) in world coordinates
    vec3 fragPosition = vec3(modelMat * vec4(fragVert, 1));
    
    //calculate the vector from this pixels surface to the light source
    vec3 surfaceToLight = light.position - fragPosition;

    //calculate the cosine of the angle of incidence
    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);

    //calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: light.intensities
	// 3. The reflection coefficient(ka for Ambient & kd for Diffuse) of the surface
    vec4 surfaceColor = vec4(1.0, 1.0, 1.0, 1.0);

	//�Խ̳�Դ������޸ģ�������һ�+vec4(vec3(0.2, 0.2, 0.2)*surfaceColor.rgb, 0.0)�����Ӷ������˻�����
    gl_FragColor = vec4(brightness * light.intensities * surfaceColor.rgb * kd.rgb, surfaceColor.a) + vec4(vec3(0.2, 0.2, 0.2)*surfaceColor.rgb * kd.rgb, 0.0);
	//gl_FragColor = vec4(fragTexCoord, 0.5, 1);
}