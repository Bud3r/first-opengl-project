#version 330 core

layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
	mat4 view;
	mat4 projection;
};

out vec2 UV;
out vec4 vertexColor;

uniform mat4 view2;
uniform mat4 model;


void main() 
{
	vertexColor = projection[0];
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
