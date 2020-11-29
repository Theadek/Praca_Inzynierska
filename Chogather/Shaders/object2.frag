#version 330 core
in vec3 FragPos;
in vec3 Normals;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform int number_of_texture_diffuse;
uniform int number_of_texture_specular;
uniform int number_of_texture_normal;
uniform int number_of_texture_height;

uniform vec3 viewPos;
uniform vec3 lightPosition;

vec3 lightAmbient = vec3(0.1, 0.1, 0.1);
vec3 lightDiffuse = vec3(1.0, 1.0, 1.0);
vec3 lightSpecular = vec3(1.0, 1.0, 1.0);


void main()
{
    //check bump textures or Normals
    vec3 norm;
    if(number_of_texture_normal == 0) {
        norm = normalize(Normals);
    }
    else if(number_of_texture_normal == 1) {
        norm = texture(texture_normal1, TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
    }

    //calculate positions
    vec3 lightDir = normalize(lightPosition - FragPos);
    //float diff = max(dot(norm, lightDir), 0.0);
    float diff = dot(norm, lightDir);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 225);

    //check diffuse textures
    vec3 ambient;
    vec3 diffuse;
    if(number_of_texture_diffuse == 0) {
        ambient = vec3(0.5, 0.5, 0.5);
        diffuse = lightDiffuse * diff; //TODO add material.diffuse ???
    }
    else if(number_of_texture_diffuse == 1) {
        ambient = lightAmbient * texture(texture_diffuse1, TexCoords).rgb;
        diffuse = lightDiffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
    }

    //check specular textures
    vec3 specular = vec3(0.0, 0.0, 0.0);
    if(number_of_texture_specular == 0) {
        specular = lightSpecular * spec * vec3(0.6, 0.6, 0.6); //TODO add material.specular ???
    }
    else if(number_of_texture_specular == 1) {
        specular = lightSpecular * spec * texture(texture_specular1, TexCoords).rgb;
    }

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result.r * 0.5f, result.g * 0.5f, result.b * 1.0f, 1.0);

}