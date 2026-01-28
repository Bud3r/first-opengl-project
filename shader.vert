#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec4 verPos;
out vec2 UV;

uniform float offset;
uniform mat4 transform;


void main() 
{
	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	verPos = gl_Position;
	UV = aUv;
}
