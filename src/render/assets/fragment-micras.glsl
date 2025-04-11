#version 330 core
out vec4 FragColor;

in vec3 ourColor;

void main () {
    //FragColor = vec4(15.0f/255.0f, 125.0f/255.0f, 15.0f/255.0f, 1.0f);
    FragColor = vec4(ourColor, 1.0f);
}
