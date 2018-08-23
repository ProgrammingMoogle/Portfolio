#version 330
uniform vec4 color;
uniform float alpha;

layout (location = 5) in vec2 CUV;

out vec4 FragColor;

void main() {
  if( (CUV.x * CUV.x) + (CUV.y * CUV.y) > 1 ) {
    discard;
  }
  
  FragColor = vec4(color.rgb, alpha);
}