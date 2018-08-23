#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D samp;

void main()
{
   FragColor = vec4(texture(samp, TexCoord.st).rgb, 1.0);
}
