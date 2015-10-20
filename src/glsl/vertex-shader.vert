#version 150

in vec3 vert;
in vec3 vertNormal;
in vec2 vertTexCoord;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 fragNormal;

void main() {
    // Pass some variables to the fragment shader
    fragTexCoord = vertTexCoord;
    fragNormal = vertNormal;
    fragVert = vert;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(vert, 1);
}