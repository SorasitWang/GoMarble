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
#include <stdio.h>

struct Linear {
    float A;
    float B;
    float C;
} ;
class Wood {
	

public :
	Wood() {

	};
    Linear eq = Linear();
    int idx[2] = { 0,1 };
    float slope;
	std::pair<glm::vec3, glm::vec3> pieces;
	unsigned int VAO, VBO,EBO;
    std::vector<float> vertices;

	void init(Shader shader,glm::vec3 start) {
        
        vertices.push_back(start.x); vertices.push_back(start.y); vertices.push_back(0.0f);
        pieces.first = start;
        vertices.push_back(start.x); vertices.push_back(start.y); vertices.push_back(0.0f);
        /*float vertices[] = {
            // positions         // colors
             start.x, start.y, 0.0f, // bottom right
             end.x ,end.y, 0.0f,    // bottom left
 
        };*/
       
        shader.use();
       
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

       
        // color attribute
       	
	}

    
	void draw(Shader shader,glm::vec3 end) {
        vertices[3] = end.x; vertices[4] = end.y; vertices[5] = 0.0f;
        pieces.second = end;
        slope = (vertices[1] - end.y) / (vertices[0] - end.x);
        shader.use();
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(this->VAO);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}

    void draw(Shader shader) {
        shader.use();
        glBindVertexArray(this->VAO);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

    }




};