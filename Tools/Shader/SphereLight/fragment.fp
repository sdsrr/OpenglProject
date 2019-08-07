#version 330

in vec3 normal;
in vec4 position;
in mat4 modelMatrix;

struct Light
{
    float radius;
    vec3 color;
    vec4 position;
};

const int MaxLightnum = 3;
uniform Light lights[MaxLightnum];
uniform vec4 environmentColor;

out vec4 vFragColor;

void main(void)
{
    for (int i = 0 ; i < MaxLightnum; i++)
    {
        Light li = lights[i];
        mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
        vec3 lightDirection = li.position.xyz - position.xyz;
        float atten = (li.radius - length(vec3(lightDirection.xyz))) / li.radius;
        float diffuse = dot(normalize(lightDirection), normalize(normal * normalMatrix));
        vec3 color = max(0, atten) * li.color * max(0, diffuse) * vec3(1,1,1);

        vFragColor += vec4(color, 1);
    }
}
