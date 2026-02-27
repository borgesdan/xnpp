$input a_position, a_color0, a_texcoord0
$output v_color0, v_texcoord0

#include <bgfx_shader.sh>

void main()
{
    // Transforma a posição do vértice para o espaço da tela
    // u_viewProj é uma matriz interna do bgfx baseada no setViewTransform
    gl_Position = mul(u_viewProj, vec4(a_position, 1.0));

    // Passa a cor e o UV para o Fragment Shader
    v_color0 = a_color0;
    v_texcoord0 = a_texcoord0;
}