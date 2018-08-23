#version 330
uniform mat4 transform;
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
out vec2 TexCoord;
void main() {
  gl_Position = transform * vec4(pos,1);
  TexCoord = tex;
}