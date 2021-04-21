#version 420

in vec4 position;
in vec3 normal;

out vec4 gPosition;
out vec4 gNormal;

void main(void)
{
    gPosition = position;
    gNormal = vec4(normal, 1);
}
