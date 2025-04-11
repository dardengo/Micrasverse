#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
//layout (location=2) in vec3 aColor;

//out vec3 ourColor;
out vec3 FragPosition;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
    FragPosition = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    //The order of multiplication is important!
    gl_Position = projection * view * vec4(FragPosition, 1.0);

    //ourColor = aColor;
}
