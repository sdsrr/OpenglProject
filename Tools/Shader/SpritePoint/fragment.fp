#version 330

in vec4 color;
uniform sampler2D colorMap00;
out vec4 vFragColor;

void main(void)
{
    vFragColor = color*texture(colorMap00, gl_PointCoord);
}
