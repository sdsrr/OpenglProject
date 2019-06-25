#version 330

in vec2 texcoord;
in vec4 shadowSpacePos;
uniform sampler2D colorMap00;
uniform sampler2D colorMap01;

out vec4 vFragColor;

float calcShadow(vec4 position)
{
    vec3 pos = position.xyz / position.w;
    pos = pos * 0.5 + 0.5;
    float shadow = texture(colorMap01, pos.xy).r;
    return step(shadow+0.0025, pos.z);
}

vec4 lerp(vec4 a, vec4 b, float percent)
{
    return a * (1 - percent) + b * percent;
}

void main(void)
{
    vec4 col = texture(colorMap00, texcoord);
    float shadow = calcShadow(shadowSpacePos);
    vFragColor = lerp(col, vec4(0,0,0,1), shadow);
}
