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
#include "Util.h"
class Marble {
public:
	Marble() {

	}
	unsigned int VAO, VBO, EBO;
	float radius = 0.1;
	
	float h = 1.0f;
	glm::vec3 position = glm::vec3(-0.8f, h, 0.0f);
	glm::vec3 velocity = glm::vec3(0.0f,0.0f,0.0f);
	float g = -0.3;
	float slope;
	float t = 0.0f;
	float velo;


	void init(Shader shader) {
		float vertices[360 / 5 * 3 + 3];
		int indx[73 * 3], idx = 3;
		vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f;
		for (int i = 0; i < 360; i += 5) {
			vertices[idx] = radius * glm::cos(glm::radians((float)i)); vertices[idx + 1] = radius * glm::sin(glm::radians((float)i)); vertices[idx + 2] = 0.0f;
			idx += 3;
		}
		idx = 0;
		
		for (int i = 1; i <= 72; i++) {
			indx[idx] = 0; indx[idx + 1] = i; indx[idx + 2] = (i + 1) > 72 ? 1 : i + 1;
			idx += 3;
		}
		
		shader.use();
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//เชื่อม


		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indx), indx, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

	}

	void draw(Shader shader,float deltaTime,std::vector<Wood> woods) {
		shader.use();
		
		t = deltaTime;
		//std::cout << g *  << std::endl;
		position += velocity * deltaTime;
		glm::mat4 model = glm::mat4(1.0f);

		onWood(woods);
		model = glm::translate(model, position);
		shader.setMat4("model", model);

		glBindVertexArray(this->VAO);

		glDrawElements(GL_TRIANGLES, 73 * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

	void onWood(std::vector<Wood> wood) {
		for (Wood w : wood) {
			glm::vec3 tmp = glm::vec3(position.x + 1.0f, position.y + (-1/w.slope), position.z);
			auto re = lineIntersection(w.pieces.first,w.pieces.second,this->position,tmp);
			std::cout << w.slope << "  " <<glm::distance(re, position)<<std::endl;
			float size = sqrt(g * (position.y-h) * 2);
			std::cout << re.x <<" " <<re.y << std::endl;
			if ((re.x != FLT_MAX || re.y != FLT_MAX) && glm::distance(re,position)<this->radius ) {
				
				float angle = glm::atan(w.slope);
				velocity.x = size * glm::cos(angle); velocity.y = size * glm::sin(angle);
				std::cout << "Yes" << std::endl;
				//return;
			}
			else {
				
				velocity.y += g * t;
			}
		}
		if(wood.size()==0)
		velocity.y += g * t;
		//return false;
	}


};