#version 330 core
out vec4 FragColor;
in vec3 pos;
uniform vec3 color;
uniform vec3 center;
uniform float rad;
void main()
{
    vec3 tmp = pos;

    vec3 colorFade = 2.3* color * ( max(sqrt(pow(pos.x-center.x,2) + pow(pos.y-center.y,2) ),0.04)/rad  +  vec3(0.9,0,0));
    FragColor =vec4(colorFade, 1.0f);
    
    }