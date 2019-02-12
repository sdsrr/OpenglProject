#version 330

uniform samplerCube colorMap;
in vec3 texCoord;
out vec4 vFragColor;
void main(void)
{
    vFragColor = texture(colorMap, texCoord);
}
