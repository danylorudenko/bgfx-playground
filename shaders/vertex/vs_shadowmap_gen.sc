$input a_position

#include <bgfx_shader.sh>

void main()
{
	mat4 concat = mul(u_viewProj, u_model[0]);
	gl_Position = mul(concat, vec4(a_position, 1.0));
}