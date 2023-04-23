#version 410 core

in vec3 Normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform int options;
uniform vec3 ourFragColor;

void main()
{
    vec3 result;
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - viewPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    switch (options) {
        case 1: // wireframe
        case 0: // default
            result = ourFragColor;
            break;
        case 2: // flat
            result = (ambient + diffuse) * ourFragColor;
            break;
        default: // phong shading
        case 3: // phong
            result = (ambient + diffuse + specular) * ourFragColor;
            break;
        case 5:
            result = (ambient) * ourFragColor;
            break;
    }
    fragColor = vec4(result, 1.0);
}

