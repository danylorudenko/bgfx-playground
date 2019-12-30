$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(mainTexture, 0);

void main()
{
	gl_FragColor = v_color0/* * texture2D(mainTexture, v_texcoord0)*/;
}
