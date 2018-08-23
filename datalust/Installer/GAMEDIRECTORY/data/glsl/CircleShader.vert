#version 330
uniform mat4 transform;
layout (location = 0) in vec3 pos;
layout (location = 2) in vec2 cuv;

out vec2 CUV;

void main() {
  gl_Position = transform * vec4(pos,1);
  CUV = cuv;
}