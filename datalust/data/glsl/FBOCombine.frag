#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D input0;
uniform float     scale0;

uniform sampler2D input1;
uniform float     scale1;

void main()
{
  FragColor = (scale0 * texture(input0, TexCoord)) + (scale1 * texture(input1, TexCoord));
}