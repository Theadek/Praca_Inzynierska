#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normals; 
in vec3 FragPos; 

uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
uniform sampler2D bump_tex;
uniform vec3 viewPos;
uniform vec3 lightPosition;
uniform vec3 ambientModel;
uniform vec3 diffuseModel;
uniform vec3 specularModel;

vec3 lightAmbient = vec3(0.2f, 0.2f, 0.2f);
vec3 lightDiffuse = vec3(0.5f, 0.5f, 0.5f);
vec3 lightSpecular = vec3(1.0f, 1.0f, 1.0f);





void main()
{

       vec3 ambient = lightAmbient * texture(diffuse_tex, TexCoords).rgb;
       vec3 norm = texture(bump_tex, TexCoords).rgb;
       norm = normalize(norm * 2.0 - 1.0);
       vec3 lightDir = normalize(lightPosition - FragPos);
       float diff = max(dot(norm, lightDir), 0.0);
       vec3 diffuse = lightDiffuse * diff * texture(diffuse_tex, TexCoords).rgb;

       vec3 viewDir = normalize(viewPos - FragPos);
       vec3 reflectDir = reflect(-lightDir, norm);
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), 225);
       vec3 specular = lightSpecular * spec * texture(specular_tex, TexCoords).rgb;
       vec3 result = ambient + diffuse + specular;
       FragColor = vec4(result, 1.0);

       //code which work with model without textures
       //---------------------------
       //vec3 ambient = lightAmbient * ambientModel;
       //vec3 norm = normalize(Normals);
       //vec3 lightDir = normalize(lightPosition - FragPos);
       //float diff = max(dot(norm, lightDir), 0.0);
       //vec3 diffuse = lightDiffuse * (diff * diffuseModel);

       //vec3 viewDir = normalize(viewPos - FragPos);
       //vec3 reflectDir = reflect(-lightDir, norm);
       //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 225);
       //vec3 specular = lightSpecular * (spec * specularModel);
       //vec3 result = ambient + diffuse + specular;
       //FragColor = vec4(result, 1.0);

}