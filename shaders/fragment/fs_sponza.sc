$input v_normal, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(u_Texture,  0);

void main()
{
	vec3 n = normalize(v_normal);
	float ndotl = dot(n, vec3(1.0, 0.0, 0.0));
	gl_FragColor = texture2D(u_Texture, v_texcoord0) * ndotl;
}
