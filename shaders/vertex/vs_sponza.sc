$input a_position, a_normal, a_tangent, a_bitangent, a_texcoord0
$output v_normal, v_texcoord0

#include <bgfx_shader.sh>

uniform mat4 u_RAW[2];
SAMPLER2D(u_Texture,  0);

void main()
{
	mat4 concat = mul(u_viewProj, u_model[0]);
	gl_Position = mul(concat, vec4(a_position, 1.0));
	
	v_normal = a_normal;
	v_texcoord0 = a_texcoord0;
}
