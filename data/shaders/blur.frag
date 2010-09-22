uniform sampler2D tex;
uniform float intensity;

const float texDimension = 50.0;
const float texScaler =  1.0/texDimension;
const float texOffset = -0.5/texDimension;

void main(void)
{
    vec4 color = vec4(0.0,0.0,0.0,0.0);

    const float gauss0 = 1.0/32.0;
    const float gauss1 = 5.0/32.0;
    const float gauss2 =15.0/32.0;
    const float gauss3 =22.0/32.0;
    const float gauss4 =15.0/32.0;
    const float gauss5 = 5.0/32.0;
    const float gauss6 = 1.0/32.0;

    vec4 gaussFilterV[7];
    gaussFilterV[0]  = vec4( -3.0*texScaler , 0.0, 0.0, gauss0).yxzw;
    gaussFilterV[1]  = vec4( -2.0*texScaler , 0.0, 0.0, gauss1).yxzw;
    gaussFilterV[2]  = vec4( -1.0*texScaler , 0.0, 0.0, gauss2).yxzw;
    gaussFilterV[3]  = vec4(  0.0*texScaler , 0.0, 0.0, gauss3).yxzw;
    gaussFilterV[4]  = vec4( +1.0*texScaler , 0.0, 0.0, gauss4).yxzw;
    gaussFilterV[5]  = vec4( +2.0*texScaler , 0.0, 0.0, gauss5).yxzw;
    gaussFilterV[6]  = vec4( +3.0*texScaler , 0.0, 0.0, gauss6).yxzw;

    vec4 gaussFilterH[7];
    gaussFilterH[0]  = vec4( -3.0*texScaler , 0.0, 0.0, gauss0);
    gaussFilterH[1]  = vec4( -2.0*texScaler , 0.0, 0.0, gauss1);
    gaussFilterH[2]  = vec4( -1.0*texScaler , 0.0, 0.0, gauss2);
    gaussFilterH[3]  = vec4(  0.0*texScaler , 0.0, 0.0, gauss3);
    gaussFilterH[4]  = vec4( +1.0*texScaler , 0.0, 0.0, gauss4);
    gaussFilterH[5]  = vec4( +2.0*texScaler , 0.0, 0.0, gauss5);
    gaussFilterH[6]  = vec4( +3.0*texScaler , 0.0, 0.0, gauss6);


    if (texture2D(tex, vec2(gl_TexCoord[0])) != vec4(0.0,0.0,0.0,0.0)) {
        for (int i=0;i<7;i++) color += texture2D(tex, gl_TexCoord[0].st + gaussFilterV[i].xy) * gaussFilterV[i].w * gl_Color;
        color = color*0.5;
        for (int i=0;i<7;i++) color += texture2D(tex, gl_TexCoord[0].st + gaussFilterH[i].xy) * gaussFilterH[i].w * gl_Color;
        color = color*0.5;
        gl_FragColor = color*intensity + texture2D(tex, vec2(gl_TexCoord[0]))*gl_Color*(1.0-intensity);
    }
    else {
        gl_FragColor = gl_Color;
    }
}
