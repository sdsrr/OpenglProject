#version 330

in vec2 texCoords;
in vec4 fragCol;
out vec4 vFragColor;

uniform sampler2D colorMap;
uniform vec4 color;

void main(void)
{
    vFragColor = color+texture(colorMap, texCoords.st);
}
