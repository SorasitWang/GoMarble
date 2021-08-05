#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;


out vec3 fColor;
uniform vec3 direction;


void main() {    
    gl_Position = position + direction*2   ;
    EmitVertex();   
    gl_Position = position;
    EmitVertex();
    fColor = vec3(0.8,0.8,0.8);
    EndPrimitive();
}