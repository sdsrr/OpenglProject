#version 330

uniform vec4 diffuseColor;
uniform vec3 lightDirection;

in vec4 color;
in vec3 normal;

out vec4 vFragColor;

void main(void)
{
    float diffuse = max(0, dot(normalize(normal), normalize(lightDirection)));
    vFragColor = diffuseColor * diffuse;
}
