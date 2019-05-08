#version 330

precision highp float;

layout (triangles) in;
layout (line_strip, max_vertices = 8) out;

uniform float delta;

in VertexData
{
    vec4 color;
}vertex[];

out vec4 color;

void main(void)
{
    vec3 a = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
    vec3 b = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
    vec3 normal = normalize(cross(a, b));
    color = vertex[0].color;

    //center normal
    vec4 center = vec4(0,0,0,0);
    for (int i = 0; i < gl_in.length(); i++)
        center += gl_in[i].gl_Position;
    gl_Position = center / gl_in.length();
    EmitVertex();

    gl_Position += vec4(normal, 0) * delta;
    EmitVertex();
    EndPrimitive();

    // vertex normal
    for (int i = 0 ; i < gl_in.length(); i++)
    {
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
        gl_Position = gl_in[i].gl_Position + vec4(normal, 0) * delta;
        EmitVertex();
        EndPrimitive();
    }
}
