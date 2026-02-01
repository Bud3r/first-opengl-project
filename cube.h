#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_program.h"


using namespace glm;


class Cube
{
public:
	Cube(ShaderProgram* pShaderProgram);
	~Cube();
	void Draw();
	vec3 mPosition;
	vec4 mModulate;
	ShaderProgram *mShaderProgram;
private:
	unsigned int mVAO;
	unsigned int mVBO;
};


Cube::Cube(ShaderProgram* pShaderProgram) {
	mPosition = vec3(0.0);
	mModulate = vec4(1.0f);
	mShaderProgram = pShaderProgram;

	float vertices[] = {
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Cube::~Cube() {
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
}

void Cube::Draw() {
	glBindVertexArray(mVAO);
	mShaderProgram->use();
	mat4 model(1.0f);
	model = translate(model, mPosition);
	glUniformMatrix4fv(mShaderProgram->getLocation("model"), 1, GL_FALSE, value_ptr(model));
	glUniform4fv(mShaderProgram->getLocation("modulate"), 1, value_ptr(mModulate));
	glDrawArrays(GL_TRIANGLES, 0, 3);
}