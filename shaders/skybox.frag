#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform bool grayscale;

void main()
{    
    if (grayscale)
    {
        FragColor = texture(skybox, TexCoords);
        float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
        FragColor = vec4(average, average, average, 1.0);
    }
    else
    {
        FragColor = texture(skybox, TexCoords);
    }
}