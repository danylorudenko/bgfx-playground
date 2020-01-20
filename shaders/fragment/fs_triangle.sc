$input v_color0

#include <bgfx_shader.sh>

SAMPLER2D(u_Texture, 0);

void main()
{
	gl_FragColor = v_color0/* * texture2D(mainTexture, v_texcoord0)*/;
}
