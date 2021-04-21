#version 330

//使用tbo时,采样器和采样函数和普通不一样
uniform samplerBuffer colorMap00;
uniform int maxWidth;
uniform int maxHeight;

in vec2 texCoord;
out vec4 vFragColor;

void main(void)
{
    //采样值 offset = x*(maxWidth-1) + y*(maxHeight-1)*maxWidth
    int offset = int(texCoord.x * (maxWidth-1)) + int(max(0, (texCoord.y * (maxHeight-1)) * maxWidth));
    vFragColor = texelFetch(colorMap00, offset);
}
