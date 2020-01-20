$input v_normal, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_Texture,  0);

void main()
{
	vec3 n = normalize(v_normal);
	float ndotl = dot(n, vec3(1.0, 0.0, 0.0));
	gl_FragColor = texture2D(s_Texture, v_texcoord0) * ndotl;
}
