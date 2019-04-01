#version 330

in vec2 texCoord;
out vec4 vFragColor;

uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform sampler2D texture04;
uniform sampler2D texture05;

void main(void)
{
    vec4 col00 = texture(texture00, texCoord);
    vec4 col01 = texture(texture01, texCoord);
    vec4 col02 = texture(texture02, texCoord);
    vec4 col03 = texture(texture03, texCoord);
    vec4 col04 = texture(texture04, texCoord);
    vec4 col05 = texture(texture05, texCoord);
    vFragColor = (col00 + col01 + col02 + col03 + col04 + col05) / 6.0f;
}


