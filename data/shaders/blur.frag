// RadialBlur - Shader
 
uniform sampler2D Texture;   // scene texture
vec2 radial_size = vec2(1.0/960.0, 1.0/600.0);    // texel size
 
float radial_blur = 0.1;   // blur factor
float radial_bright = 1.0; // bright factor
 
uniform vec2 radial_origin;  // blur origin
 
void main(void) {
    vec2 TexCoord = vec2(gl_TexCoord[0]);

    vec4 SumColor = vec4(0.0, 0.0, 0.0, 0.0);
    TexCoord += radial_size * 0.5 - radial_origin;

    for (int i = 0; i < 14; i++) {
        float scale = 1.0 - radial_blur * (float(i) * 0.0625);
        SumColor += texture2D(Texture, TexCoord * scale + radial_origin)*(1.0 - float(i) * 0.0625);
    }

    gl_FragColor = SumColor * 0.2 * radial_bright;
}
