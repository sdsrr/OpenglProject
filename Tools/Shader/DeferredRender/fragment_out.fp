#version 420

in vec2 texCoord;
out vec4 vFragColor;
uniform sampler2D colorMap00;
uniform sampler2D colorMap01;
uniform vec3 cameraPosition;


const int MaxLightNum = 500;
struct Light
{
    float radius;
    vec3 position;
    vec3 color;
};
layout (std140, binding=0) uniform ubo
{
    Light[MaxLightNum] lights;
};

void main(void)
{
    vec3 result = vec3(0,0,0);
    vec4 position = texture(colorMap00, texCoord);
    vec3 normal = normalize(texture(colorMap01, texCoord).rgb);
    vec3 viewDir = normalize(cameraPosition - position.xyz);

    for (int i = 0; i < MaxLightNum; i++)
    {
        Light li = lights[i];
        vec3 lightDir = li.position - position.xyz;

        //diffuse
        float atten = (li.radius - length(vec3(lightDir.xyz))) / li.radius;
        float diffuse = dot(normalize(lightDir), normal);
        vec3 diffuseCol = li.color * max(0, diffuse);

        //specular
        vec3 reflectDir = reflect(lightDir, normal);
        float specular = pow(max(dot(viewDir, normalize(reflectDir)), 0), 32);
        vec3 specularCol = specular * li.color * 0.5f;

        result += max(0, atten) * (diffuseCol + specularCol);
    }
    vFragColor = vec4(result, 1);
}
