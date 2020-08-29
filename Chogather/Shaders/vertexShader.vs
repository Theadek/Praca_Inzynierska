#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 1) in vec3 aNormal;
out vec2 TexCoords;
out vec3 Normals;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoords = aTex;
    Normals = mat3(transpose(inverse(model))) * aNormal;  
}