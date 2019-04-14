#version 330

uniform sampler2D colorMap00;
uniform float brightLimit;

in vec2 texCoord;
out vec4 brightColor;

void main(void)
{
    vec4 baseColor = texture(colorMap00, texCoord);
    vec3 color = max(baseColor - vec4(brightLimit), vec4(0)).rgb;
    float bright = dot(color, vec3(1));
    brightColor.rgb = mix(vec3(0), vec3(0.6), bright);
    brightColor.a = 1;
}
