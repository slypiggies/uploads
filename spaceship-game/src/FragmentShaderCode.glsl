#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPosition; 

uniform sampler2D textureDiffuse;
uniform vec3 lightPosition; 
uniform vec3 viewPosition; 
uniform vec3 lightColor;
uniform vec3 lightPosition1; 
uniform vec3 lightColor1;
uniform sampler2D textureSampler;
uniform int normalMappingFlag;

void main()
{
    vec3 Normal1 = normalize(Normal);
    if (normalMappingFlag == 1)
    {
        vec3 Normal2 = Normal;
        Normal2 = normalize(texture(textureSampler, TexCoord).rgb * 2.0 - 1.0);
        Normal2.z -= 1.0;
        Normal2 *= 0.45;
        Normal1 += Normal2;

        //Normal1 = normalize(texture( textureSampler, TexCoord ).rgb*2.0 - 1.0);
        //Normal1 = normalize(texture( textureSampler, vec2(TexCoord.x,-TexCoord.y) ).rgb*2.0 - 1.0);
        //Normal1 += normalize(texture(textureSampler, TexCoord).rgb) - vec3(0.0,0.0,1.0);
        //Normal1 = (texture(textureSampler, TexCoord).rgb * 2.0 - 1.0);
        //Normal1 = normalize(texture(textureSampler, TexCoord).rgb);
        //Normal1 += texture(textureSampler, TexCoord).rgb;
        //Normal1.y = Normal2.y;
        //Normal1.z = Normal2.z;
     
        Normal1 = normalize(Normal1);
    }


    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    vec3 norm = Normal1;
    vec3 lightDir = normalize(lightPosition - FragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPosition - FragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  




    
    float ambientStrength1 = 0.5;
    vec3 ambient1 = ambientStrength1 * lightColor1;
  	
    vec3 norm1 = Normal1;
    vec3 lightDir1 = normalize(lightPosition1 - FragPosition);
    float diff1 = max(dot(norm1, lightDir1), 0.0);
    vec3 diffuse1 = diff1 * lightColor1;
    
    float specularStrength1 = 0.5;
    vec3 viewDir1 = normalize(viewPosition - FragPosition);
    vec3 reflectDir1 = reflect(-lightDir1, norm1);  
    float spec1 = pow(max(dot(viewDir1, reflectDir1), 0.0), 32);
    vec3 specular1 = specularStrength1 * spec1 * lightColor1;  



        
    vec3 result = (ambient + diffuse + specular) + (ambient1 + diffuse1 + specular1);
    FragColor = vec4(result, 1.0) * texture(textureDiffuse, TexCoord);
}