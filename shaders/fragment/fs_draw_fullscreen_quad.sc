$input v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(u_DebugTexture, 0);

void main()
{
	gl_FragColor = texture2D(u_DebugTexture, v_texcoord0);
}
