#version 330 core
//simple shader for the lightsource, its just a glowing cube
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(lightColor,1.0); // set alle 4 vector values to 1.0
}