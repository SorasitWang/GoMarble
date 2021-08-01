#version 330 core
out vec4 FragColor;
in float drawing;
uniform vec3 color;
void main()
{
    if (drawing == 0) discard;
    else if (drawing == 0.5) FragColor =vec4(0.3,0.8,0.3, 1.0f);
    else FragColor =vec4(color, 1.0f);
    }