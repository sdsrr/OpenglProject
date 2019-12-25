#version 410

in vec3 normal;
in vec4 position;

layout (location = 0) out vec4 vPosition;
layout (location = 1) out vec4 vNormal;

//相机远近裁剪面
uniform float nearPlane;
uniform float farPlane;

float LinearizeDepth(float depth)
{
    float z = depth * 2 - 1;//ndc
    float lineDepth = (2 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
    return lineDepth;
}

void main(void)
{
    //转化到视图空间
    vPosition = position;
    vPosition.a = LinearizeDepth(gl_FragCoord.z);
    vNormal = normalize(vec4(normal, 1));
}
