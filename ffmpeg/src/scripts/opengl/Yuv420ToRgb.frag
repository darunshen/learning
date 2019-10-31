#version 330
precision mediump float;
in vec2 texture_out;
uniform sampler2D yuv_y;
uniform sampler2D yuv_u;
uniform sampler2D yuv_v;
/*
    R =                 | 298.082    0           408.58  |   |Y  - 16  |
    G = (1/256)   *     | 298.082    -100.291    -208.12 | * |Cb - 128 |
    B =                 | 298.082    516.411     0       |   |Cr - 128 |
    YCbCr(YUV))已被放缩[0,1]区间,因此 16,128,128在程序中也应相应放缩
*/    
void main(void)
{
    vec3 yuv;
    vec3 rgb;    
    yuv.x = 1.1643 * (texture(yuv_y, texture_out).r-0.0625);
    yuv.y = texture(yuv_u, texture_out).r - 0.5;
    yuv.z = texture(yuv_v, texture_out).r - 0.5;   
    rgb =transpose(mat3( 1,       0,         1.5958,
                1,       -0.39173,  -0.8129,
                1, 2.017,  0)) * yuv;    
    gl_FragColor = vec4(rgb,1);
}