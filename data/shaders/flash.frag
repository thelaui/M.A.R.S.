uniform sampler2D texSampler;
uniform float intensity;
uniform float exposure;

void main()
{
    float alpha;
    vec4 color;
    if (texture2D(texSampler, vec2(gl_TexCoord[0])) != vec4(0.0,0.0,0.0,0.0)) {
        alpha = texture2D(texSampler, vec2(gl_TexCoord[0])).a * gl_Color.a;
        color=vec4(texture2D(texSampler, vec2(gl_TexCoord[0]))*gl_Color);
        float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
        gl_FragColor = vec4(gray*exposure, gray*exposure, gray*exposure, alpha*exposure)*intensity + texture2D(texSampler, vec2(gl_TexCoord[0]))*gl_Color*(1.0-intensity);
    }
    else {
        float gray = dot(gl_Color.rgb, vec3(0.299, 0.587, 0.114));
        gl_FragColor = vec4(gray*exposure, gray*exposure, gray*exposure, gl_Color.a*exposure)*intensity + gl_Color*(1.0-intensity);
    }
}
