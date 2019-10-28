attribute vec4 vertex_in; 
attribute vec2 texture_in;
varying vec2 texture_out;
void main(void)
{
    gl_Position = vertex_in; 
    texture_out = texture_in;
}