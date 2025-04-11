#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

//in vec3 ourColor;
in vec3 FragPosition;
in vec3 Normal;

uniform Material material;
uniform Light light;

uniform vec3 viewPosition;

void main () {
    //FragColor = vec4(15.0f/255.0f, 15.0f/255.0f, 15.0f/255.0f, 1.0f);
    //FragColor = vec4(ourColor, 1.0f);

    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPosition);
    float diffuseCte = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * (diffuseCte * material.diffuse);

    // specular
    vec3 viewDirection = normalize(viewPosition - FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float specularCte = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess * 128);
    vec3 specular = light.specular * (specularCte * material.specular);

    FragColor = vec4(vec3(ambient + diffuse + specular), 1.0);
}
