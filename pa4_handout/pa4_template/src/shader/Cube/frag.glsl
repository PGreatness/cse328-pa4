#version 410 core

in vec3 ourFragColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    vec3 result;
    // ambient
    float ambientStrength = 0.6;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - viewPos);
    result = (ambient) * ourFragColor;
    fragColor = vec4(result, 1.0);
}

