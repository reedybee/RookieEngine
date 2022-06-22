#version 330 core  
out vec4 FragColour;  

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

uniform vec3 viewPos;

uniform sampler2D albedoMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;

uniform float ambientStrength;
uniform vec3 lightColour;
uniform vec3 lightPos;

void main() {

    // calculates the roughness
    float roughness = normalize(texture(roughnessMap, texCoord).r);

    // calculates ambient lighting
    vec3 ambient = ambientStrength * lightColour;
  	
    // calculates diffuse lighting
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
    
    // calculates specular lighting
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = roughness * spec * lightColour; 
    
    // prepares the texture
    vec4 resTexture = texture(albedoMap, texCoord);

    // calculates final colour for fragment
    vec4 result = vec4((ambient + diffuse + specular), 1.0) * resTexture;
    FragColour = result;
}