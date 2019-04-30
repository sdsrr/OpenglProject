#version 330

uniform vec4 diffuseColor;

in vec4 color;
in vec3 light;
in vec3 normal;

out vec4 vFragColor;

void main(void)
{
    float diffuse = max(0, dot(normalize(normal), normalize(light)));
    vFragColor = diffuseColor * diffuse;
    vFragColor.a = 1;
}
