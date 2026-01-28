#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUv;

out vec4 verPos;
out vec2 UV;

uniform float offset;
uniform mat4 transform;
uniform mat4 cameraTransform;
uniform mat4 perspective;


void main() 
{
	gl_Position = perspective * cameraTransform * transform * vec4(aPos, 1.0);
	verPos = gl_Position;
	UV = aUv;
}
