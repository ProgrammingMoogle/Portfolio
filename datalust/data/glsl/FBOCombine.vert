#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;

out vec2 TexCoord;

void main()
{
    const float up_scale = 2;
    
    gl_Position = vec4(up_scale * pos.x, up_scale * pos.y, 0.0, 1.0);
    TexCoord = tex;
}  