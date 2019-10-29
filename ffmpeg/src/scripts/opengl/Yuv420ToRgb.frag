#version 330
precision mediump float;
in vec2 texture_out;
uniform sampler2D yuv_y;
uniform sampler2D yuv_u;
uniform sampler2D yuv_v;
void main(void)
{
    vec3 yuv;
    vec3 rgb;    
    yuv.x = texture2D(yuv_y, texture_out).r;
    yuv.y = texture2D(yuv_u, texture_out).r - 0.5;
    yuv.z = texture2D(yuv_v, texture_out).r - 0.5;
    rgb = mat3( 1,       1,         1,
                0,       -0.39465,  2.03211,
                1.13983, -0.58060,  0) * yuv;    
    gl_FragColor = vec4(rgb, 1);
}