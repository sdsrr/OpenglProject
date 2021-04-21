#version 330

uniform sampler2D colorMap00;
uniform float brightLimit;

in vec2 texCoord;
out vec4 brightColor;

void main(void)
{
    vec3 baseColor = texture(colorMap00, texCoord).rgb;
    vec3 color = max(baseColor - vec3(0.4), vec3(0));
    float bright = dot(color, vec3(1));
    brightColor.rgb = mix(vec3(0), baseColor, bright);
    brightColor.a = 1;
}
