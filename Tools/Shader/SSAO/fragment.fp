#version 330

uniform sampler2D colorMap00; //position
uniform sampler2D colorMap01; //normal
uniform sampler2D colorMap02; //ssao

uniform vec3 lightDirection;
uniform vec4 diffuseColor;
uniform mat4 vMatrix;

in vec2 texcoord;
out vec4 vFragColor;

vec4 lerp(vec4 a, vec4 b, float percent)
{
    return b*percent + a*(1-percent);
}

void main(void)
{
    vec4 background = vec4(0,0.8,0.8,1);
    //camer dir(zero is camer location)
    vec3 dir = texture(colorMap00, texcoord).xyz - vec3(0);
    vec3 normal = texture(colorMap01, texcoord).xyz;
    float ssao = texture(colorMap02, texcoord).x;

    //calculate in view coordinate sys, light is world vector, transform to view sys vector
    mat3 matrix = mat3(transpose(inverse(vMatrix)));
    vec3 light = lightDirection * matrix;

    //diffuse
    vec4 diffuse = diffuseColor * max(dot(normalize(lightDirection), normalize(normal)), 0);

    //speculer
    vec4 speculer = vec4(0);

    vec4 color = (speculer + diffuse) * ssao;
    //frag is background which normal data is zero
    vFragColor = lerp(color, background, step(length(normal), 0));
}
