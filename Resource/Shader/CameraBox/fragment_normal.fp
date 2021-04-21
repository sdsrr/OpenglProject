#version 330

uniform vec4 diffuseColor;
out vec4 vFragColor;

void main(void)
{
    vFragColor = vec4(diffuseColor.xyz, 0.5f);
}
