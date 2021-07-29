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
#include <iostream>
class Bin {
public:
	Bin() {

	}
	unsigned int VAO, VBO, EBO;
	float size = 0.2;
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 color = glm::vec3(1.0, 0.8, 0.4);

	void init(Shader shader) {

		float v[] = {
			-size , -size, 0.0f,
			-size , size , 0.0f,
			 size , size ,  0.0f,
			 size , -size , 0.0f
		};
		int idx[] = {
			0,1,
			2,3,
			0,3
		};
		position.y = -0.75;
		shader.use();
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}

	void draw(Shader shader, float deltaTime) {
		shader.use();
		shader.setVec3("color", color);
		glm::mat4 model = glm::mat4(1.0f);

		move(deltaTime);
		model = glm::translate(model, position);
		
		shader.setMat4("model", model);

		glBindVertexArray(this->VAO);
		glLineWidth(1);
		glDrawElements(GL_LINES ,6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

	void move(float deltaTime) {
		//position.x = glm::sin((float)glfwGetTime()) * 0.85;
	}

	bool checkIn(glm::vec3 obj) {

		if (obj.y <= position.y + size && obj.y >= position.y - size 
			&& obj.x <= position.x + size && obj.x >= position.x - size) {
			std::cout << "In" << std::endl;
			return true;
		}
		return false;

	}


};