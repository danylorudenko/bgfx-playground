$input a_position, a_color0
$output v_color0

#include <bgfx_shader.sh>

uniform mat4 u_RAW[2];
SAMPLER2D(s_Texture,  0);

void main()
{
	mat4 concat = mul(u_viewProj, u_model[0]);
	gl_Position = mul(concat, vec4(a_position, 1.0));
	
	v_color0    = a_color0;
}
