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
#include "../header/bin.h"
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
	float g = -0.5;
	bool out = false;
	float mass = 1;
	float lost = 1;
	float slope;
	float t = 0.0f;
	float velo;
	bool stable = false;
	int col=-1;


	void init(Shader shader,glm::vec3 startPos) {
		position = startPos;
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

	void draw(Shader shader,float deltaTime,std::vector<Wood> woods,Bin bin) {
		if (out) return;
		shader.use();
		bin.checkIn(position);
		t = deltaTime;
		//std::cout << velocity.y  << std::endl;
		position += velocity * deltaTime;
		glm::mat4 model = glm::mat4(1.0f);

		onWood(woods);
		//std::cout << position.y << std::endl;
		if (position.x > 1.0 || position.y > 1.0) out = true;
		model = glm::translate(model, position);
		shader.setMat4("model", model);

		glBindVertexArray(this->VAO);
		glLineWidth(1);
		glDrawElements(GL_TRIANGLES, 73 * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

	void onWood(std::vector<Wood> wood) {
		float angle = abs(glm::degrees(glm::atan(velocity.y / velocity.x)));
		//if (angle == 90)angle = -90;
		std::cout << angle << std::endl;
		glm::vec3 velo = velocity;
		bool c = false;
		for (int i = 0; i < wood.size(); i++) {
			auto w = wood[i];
			if (!(position.x <= wood[i].border.maxX && position.x >= wood[i].border.minX
				&& position.y <= wood[i].border.maxY+radius && position.y >= wood[i].border.minY)) {
				continue;
			}
			for (int j = 0; j < wood[i].vertices.size() - 3; j += 3) {

				float slope = (wood[i].vertices[j + 1] - wood[i].vertices[j + 4]) / (wood[i].vertices[j] - wood[i].vertices[j + 3]);
				glm::vec3 tmp = glm::vec3(position.x + 1.0f, position.y + (-1 / slope), position.z);
				glm::vec3 start = glm::vec3(wood[i].vertices[j], wood[i].vertices[j + 1], wood[i].vertices[j + 2]);
				glm::vec3 end = glm::vec3(wood[i].vertices[j + 3], wood[i].vertices[j + 4], wood[i].vertices[j + 5]);
				auto re = lineIntersection(start, end, this->position, tmp);

				float angleWood = glm::degrees(glm::atan(slope));
				if ((re.x != FLT_MAX || re.y != FLT_MAX) && glm::distance(re, position) < this->radius) {
					c = true;
					velo = velocity;
					//std::cout << "angleWood " << angleWood << std::endl;
					float size = 1;
					size = glm::distance(velo, glm::vec3(0.0f));
					/*if (size > 0.5 && !stable)
						velo = glm::vec3(0.5) * glm::reflect(velo, glm::normalize(glm::vec3(1, -1 / slope, 0.0f)));*/
					if (col != j) {
						stable = false;
						col = j;
						//std::cout << "ss " << angle << " " << angleWood << 90 - angle - angleWood << std::endl;


						//glm::normalize(velocity);
						//glm::vec3 bounce = glm::vec3(0.1) * glm::reflect(velo, glm::normalize(glm::vec3(1, -1 / slope, 0.0f)));
						velo.x = copysign(1.0, velocity.x)*(size * glm::sin(glm::radians(90 - angle - angleWood))) * glm::cos(glm::radians(angleWood));
						velo.y = copysign(1.0, velocity.x) * (size * glm::sin(glm::radians(90 - angle - angleWood))) * glm::sin(glm::radians(angleWood));
						//std::cout << angle << " " << angleWood << " " << glm::sin(glm::radians(90 - angle - angleWood)) << std::endl;
						//std::cout << ;

					}
					else {
						stable = true;
					}
					//std::cout << size << std::endl;

					float sizeG = 0;
					if (velo.y > 0) sizeG = g * t * glm::sin(glm::radians(angleWood));
					else sizeG = g * t * glm::sin(glm::radians(angleWood));
					//std::cout << sizeG << " " << velocity.y << " " << velocity.x << " "<< glm::sin(glm::radians(angleWood))<< std::endl;
					float vfriction = w.staticFrict * g * glm::sin(glm::radians(angleWood)) * t;
					//velocity.x += -copysign(1.0, velocity.x)*vfriction * glm::cos(glm::radians(angleWood));
					//velocity.y += -copysign(1.0, velocity.y)*vfriction * glm::sin(glm::radians(angleWood));
					velo.x += sizeG * glm::cos(glm::radians(angleWood));

					velo.y += sizeG * glm::sin(glm::radians(angleWood));
					float rat = 1 - (vfriction / glm::distance(velo, glm::vec3(0.0f)));
					//std::cout << rat << std::endl;
					velo.x *= rat;
					velo.y *= rat;
					velocity = velo;
					return;
				}
				
			}
		}
	
		if (!c)
		velocity.y += g * t;
	}


};