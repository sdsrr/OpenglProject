#version 330

uniform sampler2D colorMap00;

in float r;
in vec2 texcoord;
out vec4 vFragColor;

void main(void)
{
    vFragColor = texture(colorMap00, texcoord);
}
