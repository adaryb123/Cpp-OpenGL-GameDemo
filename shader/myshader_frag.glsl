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

// 2 sources of light
uniform Light light;
uniform Light light2;

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
in vec4 normal;

// The final color
out vec4 FragmentColor;

vec3 getAmbientDiffuseAndSpecular(Light light){
    vec3 lightDirection = normalize(light.position - FragPos);

    //attenuation (it means light fades with distance)
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
    light.quadratic * (distance * distance));

    //ambient light
    vec3 ambient = light.ambient * material.ambient * light.color * attenuation;

    //diffuse light
    float diff = max(dot(normal, vec4(normalize(lightDirection),1.0)), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse) * light.color * attenuation;

    //specular light
    vec3 viewDirection = normalize(viewPosition - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, vec3(normal));
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess * 128);
    vec3 specular = light.specular * (spec * material.specular) * light.color * attenuation;

    return vec3(ambient + diffuse + specular);
}

void main(){
    // calculate values for both light sources separately, combine them with object texture
    vec3 result = getAmbientDiffuseAndSpecular(light);
    vec3 result2 = getAmbientDiffuseAndSpecular(light2);
    vec4 texture_result = texture(Texture, vec2(texCoord.x, 1 - texCoord.y));
    FragmentColor = texture_result * (vec4(result,1.0) + vec4(result2,1.0));
}