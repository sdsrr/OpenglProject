#version 330

uniform sampler2D colorMap00;
uniform sampler2D colorMap01;
uniform sampler2D colorMap02;
uniform float exposure;

in vec2 texCoord;
out vec4 vFragColor;

void main(void)
{
    vec4 baseColor = texture(colorMap00, texCoord);
    vec4 brightColor = texture(colorMap01, texCoord);
    vec4 blurColor = texture(colorMap02, texCoord);
    vec4 color = baseColor + brightColor + blurColor;
    vFragColor = 1 - exp2(-color * exposure);
    vFragColor.a = 1;
}
