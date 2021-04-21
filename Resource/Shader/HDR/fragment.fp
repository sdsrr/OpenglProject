#version 330

uniform float exposure;
uniform sampler2D colorMap00;
in vec2 texCoord;
out vec4 vFragColor;

void main(void)
{
    vec4 mainCol = texture(colorMap00, texCoord);
    vFragColor = 1.0 - exp2(-mainCol * exposure);
    vFragColor.a = 1;
}
