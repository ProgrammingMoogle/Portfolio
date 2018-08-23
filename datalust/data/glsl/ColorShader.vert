#version 330
uniform mat4 transform;
layout (location = 0) in vec3 pos;
void main() {
  gl_Position = transform * vec4(pos,1);
}