#version 330

uniform sampler2D colorMap00;

in vec2 texCoord;
out vec4 baseColor;

void main(void)
{
    baseColor = texture(colorMap00, texCoord);
}
