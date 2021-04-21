#version 410

in vec2 texcoord;
out vec4 vFragColor;

uniform sampler2D colorMap00;//position
uniform sampler2D colorMap01;//normal
uniform sampler2D colorMap02;//depth
uniform sampler2D colorMap03;//noise

uniform vec2 scene;//scene length-width
uniform mat4 projectMatrix;

layout (std140) uniform Block
{
    vec3[64] kernels;//ssao sample kernel
};

void main(void)
{
    float radius = 0.3;
    //��������ͼ����uv
    vec2 noiseUV = vec2(scene.x/4, scene.y/4) * texcoord;
    //xyƽ���������,��Ϊ���ص����߿ռ������
    vec3 randVec = texture(colorMap03, noiseUV).xyz;
    //��ͼ�ռ�����
    vec3 position = texture(colorMap00, texcoord).xyz;
    //��ͼ�ռ䷨��,���ﴹֱz��
    vec3 normal = normalize(texture(colorMap01, texcoord).xyz);


    //normal����ͼ�ռ�����,�����߿ռ��µ�randVecӳ�䵽��ͼ�ռ�(ͨ����ֱ��ϵ),�о�ת�������Ͻ�??
    vec3 tangent = normalize(randVec - normal * dot(randVec, normal));
    //��ֱnormal-tangent������������,�����Ƿ���?
    vec3 bittangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bittangent, normal);

    //�����ڱ�����
    float occlusion = 0;
    for (int i = 0 ; i < 64; i++)
    {
        vec3 kernel = kernels[i] * TBN;//���������߿ռ䵽��ͼ�ռ�
        kernel = position + kernel * radius;
        vec4 offset = vec4(kernel, 1) * projectMatrix;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 +0.5;
        //λ����ͼͨ��ndc���������ȡ���
        float depth = texture(colorMap00, offset.xy).z;
        //�������������radius��Χ,������
        float rangeCheck = smoothstep(0, 1, radius / abs(position.z - depth));
        occlusion += (depth >= kernel.z ? 1 : 0) * rangeCheck;
    }
    occlusion = 1 - occlusion/64.0;
    vFragColor = vec4(occlusion,occlusion,occlusion,1);
}
