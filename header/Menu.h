#pragma once
#ifndef _UTIL_H
#define _UTIL_H

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


enum Operation {
	NONE,
	DRAW,
	ERASE,
	ADD
};
class Menu {
public:
	Menu(float s) {
		width = s;
	}
	unsigned int VAO, VBO , iconVAO , iconVBO , iconEBO;
	float width = 0.3;
	glm::vec3 borderColor = glm::vec3(0.8, 0.0, 0.2);
	unsigned int texturePencil, textureEraser, textureBoost;
	Operation op = NONE;

	void init(Shader shader,Shader iconShader) {
		float v[] = {
			-(1 - width),-1.0f,0.0f,
			-(1 - width),1.0f,0.0f,
			-1,1.0f,0.0f,

			-1,1.0f,0.0f,
			-(1 - width),-1.0f,0.0f,
			-1,-1.0f,0.0f
		};
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		//เชื่อม

		glBindVertexArray(this->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		float vIcon[] = {
		0.5f, 0.5f, 0.0f,      1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	   0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	   -0.5f, 0.5f, 0.0f,      1.0f, 1.0f, 0.0f,   0.0f, 1.0f // top left
		};
		int idx[] = {
		  0,1,3,
		  1,2,3
		};
		glGenVertexArrays(1, &iconVAO);
		glGenBuffers(1, &iconVBO);
		glGenBuffers(1, &iconEBO);
		//เชื่อม

		glBindVertexArray(iconVAO);

		glBindBuffer(GL_ARRAY_BUFFER, iconVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vIcon), vIcon, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iconEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		stbi_set_flip_vertically_on_load(true);
		glGenTextures(1, &texturePencil);
		glBindTexture(GL_TEXTURE_2D, texturePencil);
		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture
		int width, height, nrChannels;
		unsigned char* data = stbi_load("C:\\Users\\LEGION\\source\\repos\\Marble\\res\\pencil.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}


		glGenTextures(1, &textureEraser);
		glBindTexture(GL_TEXTURE_2D, textureEraser);
		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture

		unsigned char* data2 = stbi_load("C:\\Users\\LEGION\\source\\repos\\Marble\\res\\eraser.png", &width, &height, &nrChannels, 0);
		if (data2)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data2);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}

		glGenTextures(1, &textureBoost);
		glBindTexture(GL_TEXTURE_2D, textureBoost);
		// set the texture wrapping/filtering options (on currently bound texture)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load and generate the texture

		unsigned char* data3 = stbi_load("C:\\Users\\LEGION\\source\\repos\\Marble\\res\\rocket.jpg", &width, &height, &nrChannels, 0);
		if (data3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data3);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		//iconShader.use();
		//iconShader.setInt("texture1", 0);

		
	}

	void draw(Shader areaShader,Shader textureShader) {
		areaShader.use();
		areaShader.setVec3("color", borderColor);
		glBindVertexArray(this->VAO);
		glLineWidth(1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		textureShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.85, 0.8, 0));
		model = glm::scale(model, glm::vec3(0.3f));
		
		textureShader.setMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, texturePencil);
		textureShader.setInt("texture1", 0);
		//model = 
		glBindVertexArray(iconVAO);
		glBindTexture(GL_TEXTURE_2D, texturePencil);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.85, 0.4, 0));
		model = glm::scale(model, glm::vec3(0.3f));
		
		textureShader.setMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, textureEraser);
		textureShader.setInt("texture1", 0);
		//model = 
		glBindVertexArray(iconVAO);
		glBindTexture(GL_TEXTURE_2D, textureEraser);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.85, 0, 0));
		model = glm::scale(model, glm::vec3(0.3f));

		textureShader.setMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, textureBoost);
		textureShader.setInt("texture1", 0);
		//model = 
		glBindVertexArray(iconVAO);
		glBindTexture(GL_TEXTURE_2D, textureBoost);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	Operation select(float x, float y) {
		if (y >= -0.15 && y <= 0.15) op = ADD;
		else if (y >= 0.25 && y <= 0.55) op = ERASE;
		else if (y >= 0.65 && y <= 0.95) op = DRAW;
		else op = NONE;
		switch (op)
		{
		case DRAW:
			std::cout << "draw" << std::endl;
			break;
		case ERASE:
			std::cout << "erase" << std::endl;
			break;
		case ADD:
			std::cout << "add" << std::endl;
			break;
		default:
			break;
		}
		return op;
	}

};

#endif