version 330

uniform sampler2D colorMap00;
out vec4 vFragColor;

void main(void)
{
    vFragColor = texture(colorMap00, texcoord);
}
