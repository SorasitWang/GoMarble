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
#include "../header/Booster.h"
#include <iostream>
#include "../header/Wood.h"
#include "Util.h"
class Marble {
public:
	Marble() {

	}
	unsigned int VAO, VBO, EBO,texture,diVAO,diVBO;
	float radius = 0.1;
	float count = 0;
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


	void init(Shader shader,Shader directShader,glm::vec3 startPos) {
		position = startPos;
		float vertices[360 / 5 * 5 + 5];
		int indx[73 * 3], idx = 5;
		vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = 0.0f; vertices[3] = 0.5f; vertices[4] = 0.5f;
		for (int i = 0; i < 360; i += 5) {
			vertices[idx] = radius * glm::cos(glm::radians((float)i));
			vertices[idx + 1] = radius * glm::sin(glm::radians((float)i));
			vertices[idx + 2] = 0.0f;
			vertices[idx + 3] = (glm::cos(glm::radians((float)i))+1)/2 ;
			vertices[idx + 4] = (glm::sin(glm::radians((float)i))+1)/2 ;
			idx += 5;
		}
		idx = 0;
		
		for (int i = 1; i <= 72; i++) {
			indx[idx] = 0; indx[idx + 1] = i; indx[idx + 2] = (i + 1) > 72 ? 1 : i + 1;
			idx += 3;
		}
		
		shader.use();
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;
		unsigned char* data = stbi_load("C:\\Users\\LEGION\\source\\repos\\Marble\\res\\marble.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		shader.setInt("texture1", 0);
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		//เชื่อม


		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indx), indx, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
		

		directShader.use();
		glGenVertexArrays(1, &diVAO);
		glGenBuffers(1, &diVBO);




	}

	void draw(Shader shader, Shader directShader,float deltaTime,std::vector<Wood> woods,Bin bin,Booster &boosters) {
		if (out) return;
		shader.use();
		if (bin.checkIn(position)) {
			bounceInBin(bin);
		}
		t = deltaTime;
		//std::cout << velocity.y  << std::endl;
		position += velocity * deltaTime;
		glm::mat4 model = glm::mat4(1.0f);
		boost(boosters);
		onWood(woods);
		//std::cout << position.y << std::endl;
		if (position.x > 1.0 || position.y > 1.0) out = true;
		model = glm::translate(model, position);
		shader.setMat4("model", model);
		shader.setVec3("color",glm::vec3 (0.9,0.3,0.3));
		shader.setVec3("center",position);
		shader.setFloat("rad", this->radius);
		glBindVertexArray(this->VAO);
		glLineWidth(1);
		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawElements(GL_TRIANGLES, 73 * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		float diVertices[] = {
			position.x , position.y,position.z
		};

		directShader.use();
		directShader.setVec3("direction", glm::normalize(velocity));
		float slope;
		if (velocity.x == 0)  slope = -0.00001;
		else slope = -1 * velocity.x / velocity.y;
	
		float s = 0.05;
		directShader.setVec3("slope",s*glm::normalize(glm::vec3(1/slope,1,0.0f)));
		glBindVertexArray(diVAO);

		
		glBindBuffer(GL_ARRAY_BUFFER, diVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(diVertices), diVertices, GL_STATIC_DRAW);
		glDrawArrays(GL_POINTS, 0, 1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

	}

	void onWood(std::vector<Wood> wood) {
		float angle = abs(glm::degrees(glm::atan(velocity.y / velocity.x)));
		//if (angle == 90)angle = -90;
		//std::cout << angle << std::endl;
		//std::cout << stable << std::endl;
		glm::vec3 velo = velocity;
		bool c = false;
		for (int i = 0; i < wood.size(); i++) {
			auto w = wood[i];
			if (!(position.x <= wood[i].border.maxX && position.x >= wood[i].border.minX
				&& position.y <= wood[i].border.maxY+radius && position.y >= wood[i].border.minY)) {
				continue;
			}
			for (int j = 0; j < wood[i].vertices.size() - 4; j += 4) {

				float slope = (wood[i].vertices[j + 1] - wood[i].vertices[j + 5]) / (wood[i].vertices[j] - wood[i].vertices[j + 4]);
				glm::vec3 tmp = glm::vec3(position.x + 1.0f, position.y + (-1 / slope), position.z);
				glm::vec3 start = glm::vec3(wood[i].vertices[j], wood[i].vertices[j + 1], wood[i].vertices[j + 2]);
				glm::vec3 end = glm::vec3(wood[i].vertices[j + 4], wood[i].vertices[j + 5], wood[i].vertices[j + 6]);
				auto re = lineIntersection(start, end, this->position, tmp);
				if (wood[i].vertices[j + 3] == 0 && wood[i].vertices[j + 7] == 0) continue;
				float angleWood = glm::degrees(glm::atan(slope));
				if ((re.x != FLT_MAX || re.y != FLT_MAX) && glm::distance(re, position) < this->radius) {
					count = 0;
					c = true;
					velo = velocity;
					//std::cout << "angleWood " << angleWood << std::endl;
					float size = 1;
					
					size = glm::distance(velo, glm::vec3(0.0f));
					
					if (size > 0.3 && !stable && abs(angle - abs(angleWood)) > 10) {
						velo = glm::vec3(0.5) * glm::reflect(velo, glm::normalize(glm::vec3(1, -1 / slope, 0.0f)));
						//std::cout << angle << " " << angleWood << std::endl;
					}
					else if (col != j) {
						
						stable = true;
						col = j;
						//std::cout << "ss " << angle << " " << angleWood << 90 - angle - angleWood << std::endl;


						//glm::normalize(velocity);
						//glm::vec3 bounce = glm::vec3(0.5) * glm::reflect(velo, glm::normalize(glm::vec3(1, -1 / slope, 0.0f)));
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
		
		if (!c) {
			count += t;
			if (count > 0.5) stable = false;
			velocity.y += g * t;
		}
	}

	private:
		void bounceInBin(Bin bin) {
			if (position.x + radius >= bin.position.x + bin.size) {
				position.x = bin.position.x + bin.size - radius;
				velocity.x *= -1;
			}
			else if (position.x - radius <= bin.position.x - bin.size) {
				position.x = bin.position.x - bin.size + radius;
				velocity.x *= -1;
			}
			if (position.y - radius <= bin.position.y - bin.size) {
				position.y = bin.position.y - bin.size + radius;
				velocity.y = 0;
			}

		}
		void boost(Booster &boosters) {
			if (boosters.isCol(this->position,radius)) {
				this->velocity *= glm::vec3(boosters.magnitude);
			}

		}
};