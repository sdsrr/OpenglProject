#version 330

uniform sampler2D colorMap00;

in vec2 texCoord;
out vec4 vFragColor;

void main(void)
{
    vFragColor = texture(colorMap00, texCoord);
}
