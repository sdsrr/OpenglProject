#version 330

uniform vec4 color;
uniform sampler2D colorMap00;
in vec2 texcoord;
out vec4 vFragColor;

void main(void)
{
    float alpha = texture(colorMap00, texcoord).r;
    vFragColor = vec4(color.rgb, alpha);
}
