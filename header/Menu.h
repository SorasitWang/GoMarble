#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "../header/shader_m.h"
#include "../header/stb_image.h"
#include "../header/camera.h"

class Menu {
public:
	Menu() {

	}
	unsigned int VAO, VBO;
	float width = 0.15;
	glm::vec3 borderColor = glm::vec3(0.8, 0.0, 0.2);

	void init(Shader shader, glm::vec3 startPos) {
		float v[] = {
			-(1 - width),-1.0f,0.0f,
			1 - width,1.0f,0.0f
		};
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//เชื่อม


		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}

	void draw(Shader shader) {
		shader.use();
		shader.setVec3("color", borderColor);
		glBindVertexArray(this->VAO);
		glLineWidth(1);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);
	}



};