#version 150

uniform mat4 modelMat;
uniform sampler2D tex;

uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
} light;

uniform vec3 kd;//物体的散射光反射系数

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
	//vec3 surfaceToLight = vec3(0.0, 1.0, 0.0);

    //calculate the cosine of the angle of incidence
    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);

    //calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: light.intensities
	// 3. The reflection coefficient(ka for Ambient & kd for Diffuse) of the surface
    // 4. The texture and texture coord: texture(tex, fragTexCoord)
	vec4 surfaceColor = texture(tex, fragTexCoord);

    gl_FragColor = vec4(brightness * light.intensities * surfaceColor.rgb * kd.rgb, surfaceColor.a);
	//gl_FragColor = gl_FragColor * 2.5;
	//gl_FragColor = vec4(fragNormal,1);
	//gl_FragColor = surfaceColor;
}