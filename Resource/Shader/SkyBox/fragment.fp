#version 330

uniform samplerCube colorMap00;
in vec3 texCoord;
out vec4 vFragColor;
void main(void)
{
    vFragColor = texture(colorMap00, texCoord);
}
