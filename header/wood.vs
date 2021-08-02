#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float draw;

out float drawing;
uniform mat4 model;
void main()
{
    drawing = draw;
    gl_Position = model * vec4(aPos, 1.0);
   
}