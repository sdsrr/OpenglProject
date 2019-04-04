#version 330

in vec3 texCoords;

uniform samplerCube colorMap00;
uniform vec4 diffuseColor;
out vec4 vFragColor;
void main(void)
{
    vFragColor = texture(colorMap00, texCoords);
}
