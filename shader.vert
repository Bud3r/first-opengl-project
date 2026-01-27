#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec4 verPos;
out vec2 UV;

uniform float offset;

void main() 
{
	gl_Position = vec4((aPos.x + offset) * -1f , aPos.y * -1f, aPos.z * -1f, 1.0);
	verPos = gl_Position;
	UV = aUv;
}
