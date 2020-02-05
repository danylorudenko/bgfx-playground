$input a_position
$output v_texcoord0

#include <bgfx_shader.sh>

void main()
{	
	v_texcoord0 = a_position.xy;
	gl_Position = vec4(a_position, 1.0);
}