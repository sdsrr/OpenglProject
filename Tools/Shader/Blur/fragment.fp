#version 330

in vec2 texCoord;
out vec4 vFragColor;

uniform sampler2D colorMap00;
uniform sampler2D colorMap01;
uniform sampler2D colorMap02;
uniform sampler2D colorMap03;
uniform sampler2D colorMap04;
uniform sampler2D colorMap05;

void main(void)
{
    vec4 col00 = texture(colorMap00, texCoord);
    vec4 col01 = texture(colorMap01, texCoord);
    vec4 col02 = texture(colorMap02, texCoord);
    vec4 col03 = texture(colorMap03, texCoord);
    vec4 col04 = texture(colorMap04, texCoord);
    vec4 col05 = texture(colorMap05, texCoord);
    vFragColor = (col00 + col01 + col02 + col03 + col04 + col05) / 6.0f;
}


