#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D samp;

uniform float threshold;

// discard dark pixels
void main()
{ 
    vec4 col = texture(samp, TexCoord.st);
    
    float brightness = col.r * 0.2126 + col.g * 0.7152 + col.b * 0.0722;
    
    if(brightness < threshold)
      col = vec4(brightness * col.rgb, col.a);
    
    FragColor = col;
}