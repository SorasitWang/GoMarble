#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float draw;

out float drawing;
void main()
{
    drawing = draw;
    gl_Position = vec4(aPos, 1.0);
   
}