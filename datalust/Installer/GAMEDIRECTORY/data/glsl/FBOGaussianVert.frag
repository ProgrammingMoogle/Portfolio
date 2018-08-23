#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D samp;

uniform int height;

uniform int taps;
uniform float offset[100];
uniform float kernel[100];

void main()
{
  float factor = 1.0 / height;

  vec3 col = vec3(0.0);
  
  for(int i = 0; i != taps; ++i)
  {
    vec4 sample = texture(samp, TexCoord.st + vec2(0.0, factor * offset[i]));
    col += (kernel[i] * sample.a) * sample.rgb;
  }

  FragColor = vec4(col, 1.0);
}