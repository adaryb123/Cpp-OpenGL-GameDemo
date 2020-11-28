#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

//Light properties
struct Light {
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;

//Position of camera
uniform vec3 viewPosition;

// Object material properties
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

// The vertex shader will feed this input
in vec2 texCoord;
in vec3 FragPos;

// Wordspace normal passed from vertex shader
in vec4 normal;

// The final color
out vec4 FragmentColor;

void main(){
    vec3 lightDirection = normalize(light.position - FragPos);

    //attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

    //ambient
    vec3 ambient = light.ambient * material.ambient * light.color * attenuation;

    //diffuse
    float diff = max(dot(normal, vec4(normalize(lightDirection),1.0)), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse) * light.color * attenuation;

    //specular
    vec3 viewDirection = normalize(viewPosition - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, vec3(normal));
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess * 128);
    vec3 specular = light.specular * (spec * material.specular) * light.color * attenuation;

    vec3 result = ambient + diffuse + specular;
    vec4 texture_result = texture(Texture, vec2(texCoord.x, 1 - texCoord.y));
    FragmentColor = texture_result * vec4(result,1.0);
}