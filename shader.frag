#version 330 core

out vec4 FragColor;

uniform sampler2D outTexture;
uniform vec4 modulate;

in vec2 UV;
in vec4 vertexColor;
in vec4 verPos;

void main() 
{
	// FragColor = texture(outTexture, UV);
	// FragColor = modulate;
	FragColor = modulate;
}
