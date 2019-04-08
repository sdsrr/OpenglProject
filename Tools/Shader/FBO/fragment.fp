#version 330

in vec2 texCoord;
in vec3 normal;
in vec3 light;
in vec4 color;

uniform vec4 environmentColor;
uniform sampler2D colorMap00;



void main(void)
{
    float diffuse = max(0, dot(normalize(normal), normalize(light)));
    vec4 diffuseColor = texture(colorMap00, texCoord) * diffuse;
    vec4 retCol = environmentColor + diffuseColor;
    gl_FragData[0] = retCol;//原色

    float gray = dot(retCol.rgb, vec3(0.3, 0.59, 0.11));
    gl_FragData[1] = vec4(gray,gray,gray,1);//灰度
    gl_FragData[2] = retCol * 0.5;//亮度衰减
}
