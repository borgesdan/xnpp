$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

// Define o sampler da textura (o slot 0 que usamos no setTexture)
SAMPLER2D(s_texColor, 0);

void main()
{
    // Lê o pixel da textura no ponto UV atual
    vec4 texel = texture2D(s_texColor, v_texcoord0);

    // Multiplica a cor da textura pela cor do vértice (estilo XNA)
    // Se a textura for branca, o objeto terá a cor exata de v_color0
    gl_FragColor = texel * v_color0;
}