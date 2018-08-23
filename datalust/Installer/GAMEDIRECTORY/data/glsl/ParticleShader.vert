#version 330
// constant across draw call
uniform mat4 ortho;
uniform mat4 view;

layout (location = 0) in vec3 aPos;
layout (location = 4) in mat4 instanceMat; // occupies 2, 3, 4, 5


void main(void) 
{
  gl_Position =  ortho * view * instanceMat * vec4(aPos, 1.0);
}