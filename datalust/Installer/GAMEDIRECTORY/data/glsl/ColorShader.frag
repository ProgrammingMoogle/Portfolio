#version 330
uniform vec4 color;
uniform float alpha;
in vec2 TexCoord;
out vec4 FragColor;

void main() {
  FragColor = vec4(color.rgb, alpha);
}