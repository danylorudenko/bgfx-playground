$input a_position
$output v_texcoord0

#include <bgfx_shader.sh>

vec3 OLD_RECT_COORDS[4] = {
	vec3(0.0, 0.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(1.0, 0.0, 0.0),
	vec3(1.0, 1.0, 0.0)
};

vec3 RECT_COORDS[6] = {
	vec3(1.0, 1.0, 0.0),
	vec3(0.0, 1.0, 0.0),
	vec3(0.0, 0.0, 0.0),
	vec3(0.0, 0.0, 0.0),
	vec3(1.0, 0.0, 0.0),
	vec3(1.0, 1.0, 0.0)
};

//int RECT_INDICIES[6] = { 3, 1, 0,   0, 2, 3 };

void main()
{
	vec3 v = RECT_COORDS[gl_VertexID];

	gl_Position = vec4(0.0, 0.0, 0.0, 1.0);
}