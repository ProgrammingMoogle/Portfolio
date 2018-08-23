#version 330
uniform vec4 color;
uniform float alpha;
out vec4 FragColor;

in vec2 CUV;

void main() {
  if( (CUV.x * CUV.x) + (CUV.y * CUV.y) > 1 ) {
	discard;
  }

  FragColor = vec4(color.rgb, alpha);
}