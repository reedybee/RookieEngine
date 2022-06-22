#version 330 core  
out vec4 FragColour;  

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;
uniform float time;
uniform float falloff;

void main() {
    vec3 colour;
    vec3 yellow = vec3(1.0, 0.5, 0.0);

    vec3 center = vec3(0.0);

    colour = yellow / distance(fragPos, center) * falloff;

    FragColour = vec4(colour, 1.0);
}