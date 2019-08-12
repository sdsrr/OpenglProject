#version 420

in vec3 normal;
in vec4 position;
in mat4 modelMatrix;

struct Light
{
    float radius;
    vec3 position;
    vec3 color;
};

const int MaxLightNum = 500;
layout (std140, binding=0) uniform ubo
{
    Light[MaxLightNum] lights;
};

uniform vec3 cameraPosition;
out vec4 vFragColor;

void main(void)
{
    vec3 result = vec3(0,0,0);
    mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
    vec3 normalDir = normalize(normal * normalMatrix);
    vec3 viewDir = normalize(cameraPosition - position.xyz);

    for (int i = 0; i < MaxLightNum; i++)
    {
        Light li = lights[i];
        vec3 lightDir = li.position - position.xyz;

        //diffuse
        float atten = (li.radius - length(vec3(lightDir.xyz))) / li.radius;
        float diffuse = dot(normalize(lightDir), normalDir);
        vec3 diffuseCol = max(0, atten) * li.color * max(0, diffuse);

        //specular
        vec3 reflectDir = reflect(lightDir, normalDir);
        float specular = pow(max(dot(viewDir, normalize(reflectDir)), 0), 32);
        vec3 specularCol = specular * li.color * 0.2f;

        result += diffuseCol + specularCol;
    }
    vFragColor = vec4(result, 1);
}
