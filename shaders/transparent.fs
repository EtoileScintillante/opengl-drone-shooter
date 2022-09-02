#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D ourTexture;

void main()
{   
    vec4 texColor = texture(ourTexture, TexCoords);
    if(texColor.a < 0.1) {discard;} // discard if transparent
    //else {texture(ourTexture, TexCoords);}

    FragColor = texture(ourTexture, TexCoords);
}