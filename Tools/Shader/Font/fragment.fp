#version 330;

uniform vec4 color;
uniform sample2D colorMap00;
in vec2 texcoord;
out vec4 gl_Color;

void main(void)
{
    fixed alpha = texture(colorMap00, texcoord);
    gl_Color = vec4(color, alpha);
}
