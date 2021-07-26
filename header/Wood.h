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

struct Border {
    float minX = 10;
    float maxX = -10;
    float minY = 10;
    float maxY = -10;
} ;
class Wood {
	

public :
	Wood() {

	};
    Border border = Border();
    std::vector<int> idx;
    float staticFrict = 0.5f;
    int last = 0;
    int x = 0;
    
	std::pair<glm::vec3, glm::vec3> pieces;
	unsigned int VAO, VBO,EBO;
    std::vector<float> vertices;
    float smoothnes = 0.05;
    
	void init(Shader shader,glm::vec3 start) {
       
       
        //vv[x] = start.x; vv[x + 1] = start.y; vv[x + 2] = start.z;
        //x += 3;
        if (start.x > border.maxX) border.maxX = start.x;
        if (start.y > border.maxY) border.maxY = start.y;
        if (start.x < border.minX) border.minX = start.x;
        if (start.y < border.minY) border.minY = start.y;
        vertices.push_back(start.x) ; vertices.push_back(start.y); vertices.push_back(0.0f);
        x += 3;
        pieces.first = start;
        
        idx.push_back(0); idx.push_back(1); last = 1;
       
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
        glBufferData(GL_ARRAY_BUFFER, 4 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * idx.size(), &idx[0], GL_STATIC_DRAW);
        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

       
        // color attribute
       	
	}

    
	void draw(Shader shader,glm::vec3 end) {
        
        //vv[x] = end.x; vv[x + 1] = end.y; vv[x + 2] = end.z;
        //x += 3;
        /*int vv[6] = {
            0.0,0.0,0.0,
            end.x,end.y,0.0
        };*/
        //int xx[2] = { 0,1 };
        //std::cout << glm::distance(end, glm::vec3(vertices[vertices.size() - 3], vertices[vertices.size() - 2], vertices[vertices.size() - 1])) << std::endl;
        if (glm::distance(end, glm::vec3(vertices[vertices.size() - 3], vertices[vertices.size() - 2], vertices[vertices.size() - 1])) > smoothnes) {

            if (end.x > border.maxX) border.maxX = end.x;
            if (end.y > border.maxY) border.maxY = end.y;
            if (end.x < border.minX) border.minX = end.x;
            if (end.y < border.minY) border.minY = end.y;

            this->vertices.push_back(end.x); this->vertices.push_back(end.y); this->vertices.push_back(0.0f);
            //x += 3;
            idx.push_back(last); idx.push_back(last + 1);
            last++;
        }
      
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //int iix[2] = { 0,20 };
        //int x = 5;
        //std::cout << sizeof(x) << std::endl;
        glBufferData(GL_ARRAY_BUFFER, 4 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * idx.size(), &idx[0], GL_STATIC_DRAW);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //last++;

        glBindVertexArray(this->VAO);
        glLineWidth(1);
        //glDrawArrays(GL_LINES, 0, vertices.size()/3 - 1);
        glDrawElements(GL_LINES, idx.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        /*//ix[1] = 10;
        //std::cout << ix[0] << " " << ix[1] << std::endl;
        //pieces.second = end;
        slope = (vertices[1] - end.y) / (vertices[0] - end.x);
        shader.use();
        glBindVertexArray(VAO);
       // std::cout << last << " " <<last+1 << " " << vertices.size() << std::endl;
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vv), vv, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(xx), xx, GL_STATIC_DRAW);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        last++;
        
		glBindVertexArray(this->VAO);
        glLineWidth(3);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);*/

	}

    void draw(Shader shader) {
        shader.use();
        if (vertices.size() < 6) return;
        glBindVertexArray(this->VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //int iix[2] = { 0,20 };
        //int x = 5;
        //std::cout << sizeof(x) << std::endl;
        glBufferData(GL_ARRAY_BUFFER, 4*vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*idx.size(), &idx[0], GL_STATIC_DRAW);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //last++;
        //std::cout << idx.size() << std::endl;

        glBindVertexArray(this->VAO);
        glLineWidth(1);
        //glDrawArrays(GL_LINES, 0, vertices.size()/3 - 1);
        glDrawElements(GL_LINES, idx.size()-1, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); 

    }




};