#version 330 
layout(location = 0) in vec4 vertex_in; 
layout(location = 1) in vec2 texture_in;
out vec2 texture_out;
void main(void)
{
    gl_Position = vertex_in; 
    texture_out = texture_in;
}