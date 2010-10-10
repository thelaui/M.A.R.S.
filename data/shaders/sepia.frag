uniform sampler2D texSampler;
float intensity2 = 1.0;

void main()
{
    float alpha;
    vec4 color;
    if (texture2D(texSampler, vec2(gl_TexCoord[0])) != vec4(0.0,0.0,0.0,0.0)) {
        alpha = texture2D(texSampler, vec2(gl_TexCoord[0])).a * gl_Color.a;
        color=vec4(texture2D(texSampler, vec2(gl_TexCoord[0]))*gl_Color);
        float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
        gl_FragColor = vec4(gray*1.0, gray*0.9, gray*0.7, alpha)*intensity2 + texture2D(texSampler, vec2(gl_TexCoord[0]))*gl_Color*(1.0-intensity2);
    }
    else {
        float gray = dot(gl_Color.rgb, vec3(0.299, 0.587, 0.114));
        gl_FragColor = vec4(gray*1.0, gray*0.9, gray*0.7, gl_Color.a)*intensity2 + gl_Color*(1.0-intensity2);
    }
}
