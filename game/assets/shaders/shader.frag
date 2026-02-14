#version 450 core

struct Material {
	sampler2D texture_albedo;
};

out vec4 FragColor;

uniform vec4 modulate;

uniform sampler2D texture_albedo;
uniform sampler2D texture_normal;
uniform sampler2D texture_diffuse;

in vec2 UV;
in vec4 verPos;

void main() 
{
	FragColor = modulate;
	FragColor *= texture(texture_albedo, UV);
}
