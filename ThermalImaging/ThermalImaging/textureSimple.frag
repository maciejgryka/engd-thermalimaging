uniform float blend;
uniform sampler2D tex1, tex2;

void main()
{
	vec4 texel;
	
	texel = blend*texture2D(tex1,gl_TexCoord[0].st) + (1-blend)*texture2D(tex2,gl_TexCoord[0].st);
	gl_FragColor = texel;
}