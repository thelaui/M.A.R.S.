// RadialBlur - Shader

uniform sampler2D Texture;   // scene texture
vec2 radial_size = vec2(1.0/1440.0, 1.0/900.0);    // texel size

uniform float radial_blur;   // blur factor
uniform float radial_bright; // bright factor

uniform vec2 radial_origin;  // blur origin


void main(void) {
    float size = radial_blur;

    vec2 TexCoord = vec2(gl_TexCoord[0]);

    TexCoord -= radial_origin;
    float lengthSquare = length(TexCoord);
    if(lengthSquare < 0.12)
        gl_FragColor = texture2D(Texture,  TexCoord * (1.f - (0.12f - lengthSquare) * size * 3.0f) + radial_origin);
    else
        gl_FragColor = texture2D(Texture,  TexCoord + radial_origin);
}

/*void main(void) {
    vec2 TexCoord = vec2(gl_TexCoord[0]);

    vec4 SumColor = vec4(0.0, 0.0, 0.0, 0.0);
    TexCoord += radial_size * 0.5 - radial_origin;

    for (int i = 0; i < 9; i++) {
        float scale = 1.0 - radial_blur * (float(i) * 0.1);
        SumColor += texture2D(Texture, TexCoord * scale + radial_origin)*(1.0 - float(i) * 0.1);
    }

    gl_FragColor = SumColor * 0.2 * radial_bright;
}*/
