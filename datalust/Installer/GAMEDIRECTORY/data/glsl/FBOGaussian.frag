#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D samp;

uniform float radius;

void main()
{
    vec2 radiuss[9] = vec2[](
        vec2(-radius,  radius), // top-left
        vec2( 0.0f,    radius), // top-center
        vec2( radius,  radius), // top-right
        vec2(-radius,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( radius,  0.0f),   // center-right
        vec2(-radius, -radius), // bottom-left
        vec2( 0.0f,   -radius), // bottom-center
        vec2( radius, -radius)  // bottom-right
    );

        float kernel[9] = float[](
         1.0/16, 2.0/16, 1.0/16,
         2.0/16, 4.0/16, 2.0/16,
         1.0/16, 2.0/16, 1.0/16
    );

    vec3 sampleTex[9];
    for(int i = 0; i != 9; i++)
        sampleTex[i] = vec3(texture(samp, TexCoord.st + radiuss[i]));

    vec3 col = vec3(0.0);
    for(int i = 0; i != 9; i++)
        col += sampleTex[i] * kernel[i]; 
      
    FragColor = vec4(col, 1.0);
}