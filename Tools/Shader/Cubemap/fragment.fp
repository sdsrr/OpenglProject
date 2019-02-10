#version 330

in vec3 texCoords;

uniform samplerCube cubeMap;
uniform vec4 color;
out vec4 vFragColor;
void main(void)
{
    vFragColor = texture(cubeMap, texCoords);
}
