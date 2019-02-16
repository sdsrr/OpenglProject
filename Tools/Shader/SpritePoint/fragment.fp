#version 330

in vec4 color;
uniform sampler2D colorMap;
out vec4 vFragColor;

void main(void)
{
    vFragColor = color*texture(colorMap, gl_PointCoord);
}
