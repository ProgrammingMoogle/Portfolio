#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D samp;

uniform int width;

uniform int taps;
uniform float offset[100];
uniform float kernel[100];

void main()
{
  float factor = 1.0 / width;

  vec3 col = vec3(0.0);
  for(int i = 0; i != taps; ++i)
  {
    col += kernel[i] * vec3(texture(samp, TexCoord.st + vec2(factor * offset[i], 0.0)));
  }

  FragColor = vec4(1.2 * col, 1.0);
}