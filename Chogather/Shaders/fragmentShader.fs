#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D diffuse_tex;
uniform sampler2D specular_tex;
//vec3 ambient = vec3(1.000000, 1.000000, 1.000000);
vec3 diffuse = vec3(0.800000, 0.800000, 0.800000);
vec3 specular = vec3(0.500000, 0.500000, 0.500000);
void main()
{
    //vec3 am = ambient * texture(diffuse_tex, TexCoords).rgb;
    //FragColor = mix(texture(image, TexCoords), texture(image2, TexCoords), 0.5);
    //vec3 result = diff + spec;
   // FragColor = vec4(result, 1.0);
   vec3 diff = diffuse * texture(diffuse_tex, TexCoords).rgb;
   vec3 spec = specular * texture(specular_tex, TexCoords).rgb;   
   FragColor = vec4(diff+spec, 1.0);
}