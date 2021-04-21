#version 330

in vec2 texCoords;
in vec4 fragCol;
out vec4 vFragColor;

uniform sampler2D colorMap00;
uniform vec4 diffuseColor;

void main(void)
{
    vFragColor = diffuseColor * texture(colorMap00, texCoords.st);
}
