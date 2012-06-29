uniform sampler2D Texture;
uniform sampler2D BumpMap;

uniform float Exposure;

void main(void) {
    vec2 TexCoord = vec2(gl_TexCoord[0]);
    vec2 distort  = vec2(texture2D(BumpMap, TexCoord).rb) - vec2(0.5f, 0.5f);

    gl_FragColor=vec4(texture2D(Texture, distort*0.02f +  TexCoord)*gl_Color*Exposure);
}

