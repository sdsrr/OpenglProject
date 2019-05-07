#version 330

uniform sampler2DMS colorMap00;
in vec2 textcoord;
out vec4 vFragColor;

void main(void)
{
    vec2 tmp = floor(textcoord * textureSize(colorMap00));
    vFragColor = texelFetch(colorMap00, ivec2(tmp), 4);
}
