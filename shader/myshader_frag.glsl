#version 330
        // A texture is expected as program attribute
        uniform sampler2D Texture;

        // Direction of light
        //uniform vec3 LightDirection;

        uniform vec3 lightPosition;
        uniform vec3 lightColor;
        //Position of camera
        uniform vec3 viewPosition;

        // (optional) Transparency
        uniform float Transparency;

        // (optional) Texture offset
        uniform vec2 TextureOffset;

        // The vertex shader will feed this input
        in vec2 texCoord;

        // Wordspace normal passed from vertex shader
        in vec4 Normal;

        // The final color
        out vec4 FragmentColor;

        void main() {
            // ambient
            float ambientStrength = 0.0;
            vec3 ambient = ambientStrength * lightColor;

            // diffuse
            vec3 temp1 = vec3(Normal.x,Normal.y,Normal.z);
            vec3 norm = normalize(temp1);
            vec3 lightDirection = normalize(lightPosition - vec3(texCoord,0.0));
            float diff = max(dot(norm, lightDirection), 0.0);
            vec3 diffuse = diff * lightColor;

            // specular
            float specularStrength = 0.5;
            vec3 viewDirection = normalize(viewPosition - vec3(texCoord,0.0));
            vec3 reflectDirection = reflect(-lightDirection, norm);
            float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;

            vec4 temp = vec4(ambient + diffuse + specular, 1.0);
            FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * temp;
            FragmentColor.a = Transparency;




        /*
        // Compute diffuse lighting
        float diffuse = max(dot(normal, vec4(normalize(LightDirection), 1.0f)), 0.0f);

        // Lookup the color in Texture on coordinates given by texCoord
        // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
        FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * diffuse;
        FragmentColor.a = Transparency;*/
        }
