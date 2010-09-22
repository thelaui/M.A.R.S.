uniform sampler2D texSampler;
uniform float intensity;

void main()
{
    float alpha;
    vec4 color;
    if (texture2D(texSampler, vec2(gl_TexCoord[0])) != vec4(0.0,0.0,0.0,0.0)) {
        alpha = texture2D(texSampler, vec2(gl_TexCoord[0])).a * gl_Color.a;
        color=vec4(texture2D(texSampler, vec2(gl_TexCoord[0]))*gl_Color);
        gl_FragColor = color + (color*10.0*intensity);
    }
    else {
        gl_FragColor = gl_Color + (gl_Color*10.0*intensity);
    }
}
/*uniform sampler2D tex; //main render
uniform float Exposure;

const float bloomStart = 0.85;
const vec4 bloomvec = vec4(bloomStart, bloomStart, bloomStart, bloomStart);

void main(void)
{
   	vec4 color  = texture2D(tex,gl_TexCoord[0].st)*Exposure;
	gl_FragColor =  max(vec4(0.0,0.0,0.0,0.0),color-bloomvec);
}*/
