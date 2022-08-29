#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D ourTexture;

void main()
{
    FragColor = vec4(1.0); // set all 4 vector values to 1.0 (white light source)
    FragColor = texture(ourTexture, TexCoords) * FragColor; 
}