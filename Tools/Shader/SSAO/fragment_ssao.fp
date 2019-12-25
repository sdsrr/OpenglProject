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
    //计算噪声图采样uv
    vec2 noiseUV = vec2(scene.x/4, scene.y/4) * texcoord;
    //xy随机向量作为像素点切线空间的切线方向
    vec3 randVec = texture(colorMap03, noiseUV).xyz;
    //模型空间坐标
    vec3 position = texture(colorMap00, texcoord).xyz;
    //切向空间法线,这里垂直z轴
    //vec3 normal = vec3(0, 0, 1);
    vec3 normal = normalize(texture(colorMap01, texcoord).xyz);

    //切线空间切线方向即垂直z轴的随机向量
    //vec3 tangent = normalize(randVec);
    //由randVec构造tangent,保证tangent与normal垂直
    vec3 tangent = normalize(randVec - normal * dot(randVec, normal));
    //垂直normal-tangent的有两个向量,不考虑方向?
    vec3 bittangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bittangent, normal);

    //计算遮蔽因子
    float occlusion = 0;
    for (int i = 0 ; i < 64; i++)
    {
        vec3 kernel = kernels[i] * TBN;//采样点切线空间到视图空间
        kernel = position + kernel * radius;
        vec4 offset = vec4(kernel, 1) * projectMatrix;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 +0.5;
        float depth = texture(colorMap02, offset.xy).r;
        //采样点如果超过radius范围,则舍弃
        float rangeCheck = smoothstep(0, 1, radius / abs(kernel.z - depth));
        occlusion += (depth >= kernel.z ? 1 : 0) * rangeCheck;
    }
    vFragColor = vec4(occlusion,occlusion,occlusion,1);
}
