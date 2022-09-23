#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

out vec2 TexCoords; 

uniform float magnitude;
uniform bool isDead;

// this returns a new vector that translates the position vector along the direction of the normal vector
vec4 explode(vec4 position, vec3 normal)
{
    vec3 direction = normal * magnitude; 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {   

    if (isDead) // make enemy explode if it has been shot
    {
        vec3 normal = GetNormal();

        gl_Position = explode(gl_in[0].gl_Position, normal);
        TexCoords = gs_in[0].texCoords;
        EmitVertex();
        gl_Position = explode(gl_in[1].gl_Position, normal);
        TexCoords = gs_in[1].texCoords;
        EmitVertex();
        gl_Position = explode(gl_in[2].gl_Position, normal);
        TexCoords = gs_in[2].texCoords;
        EmitVertex();
        EndPrimitive();
    }
    else // draw enemy without modifications
    {
        gl_Position = gl_in[0].gl_Position; 
        TexCoords = gs_in[0].texCoords;
        EmitVertex();
        gl_Position = gl_in[1].gl_Position; 
        TexCoords = gs_in[1].texCoords;
        EmitVertex();
        gl_Position = gl_in[2].gl_Position; 
        TexCoords = gs_in[2].texCoords;
        EmitVertex();
        EndPrimitive();
    }
}