#pragma once
#ifndef _UTIL_H
#define _UTIL_H
#include <ft2build.h>
#include <freetype/freetype.h>
//#include FT_FREETYPE_H
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
#include <map>

enum Operation {
	NONE,
	DRAW,
	ERASE,
	ADD
};
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2 Size; // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	unsigned int Advance; // Offset to advance to next glyph
};
std::map<char, Character> Characters;

class Menu {
public:
	Menu(float s) {
		width = s;
	}
	const unsigned int SCR_WIDTH = 600;
	const unsigned int SCR_HEIGHT = 600;
	unsigned int VAO, VBO , iconVAO , iconVBO , iconEBO;
	float width = 0.3;
	glm::vec3 borderColor = glm::vec3(0.8, 0.0, 0.2);
	unsigned int texturePencil, textureEraser, textureBoost;
	Operation op = NONE;
	std::map<Operation, float> iconPop;
	FT_Library ft;
	FT_Face face;
	unsigned int textVAO, textVBO;

	void init(Shader shader,Shader iconShader,Shader textShader) {
		iconPop[ADD] = 0.3; iconPop[ERASE] = 0.3; iconPop[DRAW] = 0.3;
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
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
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

		unsigned char* data2 = stbi_load("C:\\Users\\LEGION\\source\\repos\\Marble\\res\\eraser1.png", &width, &height, &nrChannels, 0);
		if (data2)
		{
			//std::cout << nrChannels << std::endl;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
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

		unsigned char* data3 = stbi_load("C:\\Users\\LEGION\\source\\repos\\Marble\\res\\rocket.png", &width, &height, &nrChannels, 0);
		if (data3)
		{
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, data3);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		//iconShader.use();
		//iconShader.setInt("texture1", 0);

		
		if (FT_Init_FreeType(&ft))
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" <<
			std::endl;
		
		if (FT_New_Face(ft, "C:\\Users\\LEGION\\source\\repos\\Marble\\res\\couri.ttf", 0, &face))
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		FT_Set_Pixel_Sizes(face, 0, 24);


		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // no byte-alignment restriction
		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
		textShader.use();
		textShader.setMat4("projection", projection);
		//glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glGenVertexArrays(1, &textVAO);
		glGenBuffers(1, &textVBO);
		glBindVertexArray(textVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		
	}

	void draw(Shader areaShader,Shader textureShader,Shader textShader,float length,float x,float y) {
		areaShader.use();
		areaShader.setVec3("color", borderColor);
		glBindVertexArray(this->VAO);
		glLineWidth(1);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		textureShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.85, 0.8, 0));
		model = glm::scale(model, glm::vec3(iconPop[DRAW]));
		
		textureShader.setMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, texturePencil);
		textureShader.setInt("texture1", 0);
		//model = 
		glBindVertexArray(iconVAO);
		glBindTexture(GL_TEXTURE_2D, texturePencil);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.85, 0.4, 0));
		model = glm::scale(model, glm::vec3(iconPop[ERASE]));
		
		textureShader.setMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, textureEraser);
		textureShader.setInt("texture1", 0);
		//model = 
		glBindVertexArray(iconVAO);
		glBindTexture(GL_TEXTURE_2D, textureEraser);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);



		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.85, 0, 0));
		model = glm::scale(model, glm::vec3(iconPop[ADD]));

		textureShader.setMat4("model", model);
		glBindTexture(GL_TEXTURE_2D, textureBoost);
		textureShader.setInt("texture1", 0);
		//model = 
		glBindVertexArray(iconVAO);
		glBindTexture(GL_TEXTURE_2D, textureBoost);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		unsigned int mouseShader;
		if (op != NONE) {
			model = glm::mat4(1.0f);
			model = glm::translate(model,glm::vec3(x,y,0.0f));
			model = glm::scale(model, glm::vec3(0.3f));
			textureShader.setMat4("model", model);
			switch (op) {
			case ADD:
				glBindTexture(GL_TEXTURE_2D, textureBoost);
				textureShader.setInt("texture1", 0);
				//model = 
				glBindVertexArray(iconVAO);
				glBindTexture(GL_TEXTURE_2D, textureBoost);
				break;
			case ERASE:
				glBindTexture(GL_TEXTURE_2D, textureEraser);
				textureShader.setInt("texture1", 0);
				//model = 
				glBindVertexArray(iconVAO);
				glBindTexture(GL_TEXTURE_2D, textureEraser);
				break;
			case DRAW:
				glBindTexture(GL_TEXTURE_2D, texturePencil);
				textureShader.setInt("texture1", 0);
				//model = 
				glBindVertexArray(iconVAO);
				glBindTexture(GL_TEXTURE_2D, texturePencil);
				break;
			}
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		}
		
		RenderText(textShader, "Avaliable length : "+ std::to_string(length), 25.0f, 25.0f, 1.0f,
			glm::vec3(0.5, 0.8f, 0.2f));
	}

	Operation select(float x, float y,bool click) {
		Operation tmp = op;
		if (y >= -0.15 && y <= 0.15) op = ADD;
		else if (y >= 0.25 && y <= 0.55) op = ERASE;
		else if (y >= 0.65 && y <= 0.95) op = DRAW;
		else op = NONE;
		
		iconPop[DRAW] = 0.3; iconPop[ERASE] = 0.3; iconPop[ADD] = 0.3;
		if (op != NONE) 
			iconPop[op] = 0.4;
		if (!click) op = tmp;
		return op;
	}
	private:
		void RenderText(Shader& s, std::string text, float x, float y, float scale,
			glm::vec3 color)
			
		{
			//std::cout << std::endl;
			// activate corresponding render state
			s.use();
			glUniform3f(glGetUniformLocation(s.ID, "textColor"),
				color.x, color.y, color.z);
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(textVAO);
			// iterate through all characters
			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				//std::cout << *c;
				Character ch = Characters[*c];
				float xpos = x + ch.Bearing.x * scale;
				float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
				float w = ch.Size.x * scale;
				float h = ch.Size.y * scale;
				// update VBO for each character
				float vertices[6][4] = {
				{ xpos, ypos + h, 0.0f, 0.0f },
				{ xpos, ypos, 0.0f, 1.0f },
				{ xpos + w, ypos, 1.0f, 1.0f },
				{ xpos, ypos + h, 0.0f, 0.0f },
				{ xpos + w, ypos, 1.0f, 1.0f },
				{ xpos + w, ypos + h, 1.0f, 0.0f }
				};
				// render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.TextureID);
				// update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, textVBO);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				// render quad
				glDrawArrays(GL_TRIANGLES, 0, 6);
				// advance cursors for next glyph (advance is 1/64 pixels)
				x += (ch.Advance >> 6) * scale; // bitshift by 6 (2^6 = 64)
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

};

#endif