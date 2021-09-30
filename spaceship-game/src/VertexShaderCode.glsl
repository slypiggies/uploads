#version 330 core
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord0;
layout (location = 2) in vec3 Normal0;

out vec2 TexCoord;
out vec3 FragPosition;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = TexCoord0;
    FragPosition = vec3(model * vec4(Position, 1.0));
    Normal = mat3(transpose(inverse(model))) * Normal0; 
    gl_Position = projection * view * model * vec4(Position, 1.0);
}