#version 330

in vec2 texCoord;
in vec3 normal;
in vec3 light;
in vec4 color;

uniform vec4 environmentColor;
uniform sampler2D colorMap00;

out vec4 gl_FragData;

void main(void)
{
    float diffuse = max(0, dot(normalize(normal), normalize(light)));
    vec4 diffuseColor = texture(colorMap00, texCoord) * diffuse;
    gl_FragData = environmentColor + diffuseColor;
}
