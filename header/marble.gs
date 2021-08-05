#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 6) out;

out vec3 fColor;
uniform vec3 direction;
uniform vec3 slope;
void main() {    
    gl_Position = gl_in[0].gl_Position + vec4(direction,0.0f)/5  ;
    EmitVertex();   
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    fColor = vec3(0.8,0.8,0.8);
  

    gl_Position = gl_in[0].gl_Position + vec4(direction,0.0f)/5  ;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(direction,0.0f)/8 + vec4(slope,0.0) ;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(direction,0.0f)/5  ;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(direction,0.0f)/8 + vec4(-slope,0.0) ;
    EmitVertex();

    EndPrimitive();
}