#version 330
uniform mat4 transform;
uniform float parallax_factor;
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
out vec2 TexCoord;
void main() {
  gl_Position = vec4(2 * pos.xy, pos.z, 1);
  
  // Texcoord = (transform * vec4(tex, 0, parallax_factor)).xy;
  TexCoord = 
    vec2(
      transform[0][0] * tex.x + transform[1][0] * tex.y + transform[3][0] * parallax_factor,
      transform[0][1] * tex.x + transform[1][1] * tex.y + transform[3][1] * parallax_factor
    );
}