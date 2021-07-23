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
	float g = -0.5;
	float mass = 1;
	float lost = 1;
	float slope;
	float t = 0.0f;
	float velo;
	int col=-1;


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
		glLineWidth(1);
		glDrawElements(GL_TRIANGLES, 73 * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

	void onWood(std::vector<Wood> wood) {
		float angle = -1 * glm::degrees(glm::atan(velocity.y / velocity.x));
		//std::cout << angle;
		for (int i = 0; i < wood.size(); i++) {
			auto w = wood[i];
			glm::vec3 tmp = glm::vec3(position.x + 1.0f, position.y + (-1 / w.slope), position.z);
			auto re = lineIntersection(w.pieces.first, w.pieces.second, this->position, tmp);

			float angleWood = glm::degrees(glm::atan(w.slope));
			//std::cout << angleWood << std::endl;
			
			if ((re.x != FLT_MAX || re.y != FLT_MAX) && glm::distance(re, position) < this->radius) {
				float size = 1;
				if (col != i) {
					col = i;
					std::cout << "ss " << angle << " " << angleWood << 90 - angle - angleWood << std::endl;
					size = glm::distance(velocity, glm::vec3(0.0f));
					velocity.x = (size * glm::sin(glm::radians(90 - angle - angleWood)) ) * glm::cos(glm::radians(angleWood));
					velocity.y = (size * glm::sin(glm::radians(90 - angle - angleWood)) ) * glm::sin(glm::radians(angleWood));
					//std::cout << ;
					
				}
				float sizeG=0;
				if (velocity.y > 0) sizeG = g * t * glm::sin(glm::radians(angleWood));
				else sizeG = g * t * glm::sin(glm::radians(angleWood));
				//std::cout << sizeG << " " << velocity.y << " " << velocity.x << " "<< glm::sin(glm::radians(angleWood))<< std::endl;
				float vfriction = w.staticFrict * g * glm::sin(glm::radians(angleWood)) * t;
				//velocity.x += -copysign(1.0, velocity.x)*vfriction * glm::cos(glm::radians(angleWood));
				//velocity.y += -copysign(1.0, velocity.y)*vfriction * glm::sin(glm::radians(angleWood));
				velocity.x += sizeG * glm::cos(glm::radians(angleWood));
				
				velocity.y += sizeG * glm::sin(glm::radians(angleWood));
				float rat = 1 - (vfriction / glm::distance(velocity, glm::vec3(0.0f)));
				//std::cout << rat << std::endl;
				velocity.x *= rat;
				velocity.y *= rat;
				return;
			}
			/*else {
				velocity.y += g * t;
			}*/
			//velocity *= glm::sin(glm::radians(90-angle-angleWood));
			
			//std::cout << w.slope << "  " <<glm::distance(re, position)<<std::endl;
			//float size = sqrt(std::max(g*(position.y-h),0.0f) * 2);
			
		}/*
			//std::cout << position.y << " " << h << " " << size<< std::endl;
			if (size < 0.005f) {
				size += g * t;
			}
			else if ((re.x != FLT_MAX || re.y != FLT_MAX) && glm::distance(re, position) < this->radius) {
				if (col != i) {
					h *= 0.8;
					col = i;
				}
				size = sqrt(std::max(g * (position.y - h), 0.0f) * 2);
			}
			else {

				velocity.y += g * t;
				continue;
			}
				
				float angle = glm::atan(w.slope);
				velocity.x = size * glm::cos(angle); velocity.y = (size+g*t) * glm::sin(angle);
				std::cout << size << " " << velocity.x << " " <<velocity.y << std::endl;
				//return;
			}*/
			
		
		
		velocity.y += g * t;
		//return false;
	}


};