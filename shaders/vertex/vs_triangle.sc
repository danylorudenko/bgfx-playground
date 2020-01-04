$input a_position, a_color0, a_texcoord0
$output v_color0, v_texcoord0

#include <bgfx_shader.sh>

// scale, rotation, translation, view, perspective
uniform mat4 myTransforms[5];

void main()
{
	//gl_Position = mul(concat, vec4(a_position, 1.0)) + vec4(0.5, 0.0, 0.0, 0.0);
	
	vec4 result = mul(vec4(a_position, 1.0), myTransforms[1]);
	gl_Position = mul(result, myTransforms[0]);
	v_color0    = a_color0;
	v_texcoord0	= a_texcoord0;
}
