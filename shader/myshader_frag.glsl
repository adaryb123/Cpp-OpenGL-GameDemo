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

// Wordspace normal passed from vertex shader
in vec3 normal;

// The final color
out vec4 FragmentColor;

void main(){
        vec3 norm = normalize(normal);
        vec3 lightDirection = normalize(light.position - vec3(texCoord,0.0));

        //ambient
        vec3 ambient = light.ambient * material.ambient * light.color;

        //diffuse
        float diff = max(dot(norm, lightDirection), 0.0);
        vec3 diffuse = light.diffuse * (diff * material.diffuse) * light.color;

        //specular
        vec3 viewDirection = normalize(viewPosition - vec3(texCoord,0.0));
        vec3 reflectDirection = reflect(-lightDirection, norm);
        float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess * 128);
        vec3 specular = light.specular * (spec * material.specular) * light.color;

        vec4 temp = vec4(ambient + diffuse + specular, 1.0);
        FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y)) * temp;
}