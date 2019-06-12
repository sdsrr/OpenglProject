#version 330

uniform sampler2D colorMap00;
out vec4 vFragColor;

in vec2 texcoord;
in vec4 color;

void main(void)
{
    vFragColor = color + texture(colorMap00, texcoord);
}
