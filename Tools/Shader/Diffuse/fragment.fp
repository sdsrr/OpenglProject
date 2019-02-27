#version 330

uniform vec4 color;
in vec4 vertexCol;
out vec4 vFragColor;

void main(void)
{
    vFragColor = vertexCol + color;
}
