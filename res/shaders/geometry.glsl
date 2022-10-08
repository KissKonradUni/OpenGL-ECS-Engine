#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * model * ((gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0)) + 0.5f - 0.5f);
    EmitVertex();
    gl_Position = projection * view * model * ((gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0)) + 0.5f - 0.5f);
    EmitVertex();
    gl_Position = projection * view * model * ((gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0)) + 0.5f - 0.5f);
    EmitVertex();
    gl_Position = projection * view * model * ((gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0)) + 0.5f - 0.5f);
    EmitVertex();
    EndPrimitive();
}