#version 330
#extension GL_EXT_texture_array: require

in vec2 texcoords;

uniform sampler2DArray colorMap;
uniform vec4 color;
uniform float time;

out vec4 vFragColor;
void main(void)
{
    vFragColor = color + texture2DArray(colorMap, vec3(texcoords, time));
}
