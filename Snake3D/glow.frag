#version 150 

uniform mat4 ModelView;
uniform vec4 MaterialColor;
uniform vec4 MaterialSpecularColor;
uniform float Shininess;

in vec3 fragPosition;
in vec3 fragNormal;

out vec4 fragColor;

void main() 
{ 
	fragColor = vec4(MaterialColor.xyz, 0.85) * Shininess;
} 

