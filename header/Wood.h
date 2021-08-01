

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
#include "Util.h"
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
    float staticFrict = -0.2f;
    int last = 0 , colLast = 0;
    int x = 0;
    glm::vec3 color = glm::vec3(1.0f, 0.5f, 0.2f);
    float thin = 4;
	std::pair<glm::vec3, glm::vec3> pieces;
    std::vector<bool> status;
	unsigned int VAO, VBO,EBO;
    std::vector<float> vertices;
    unsigned int hVAO, hVBO, hEBO;
    std::vector<float> highlight;
    std::vector<int> hIdx;
    float smoothnes = 0.5;
    float length = 0.0f;
    
	void init(Shader shader,Shader hShader,glm::vec3 start) {
       
        //vv[x] = start.x; vv[x + 1] = start.y; vv[x + 2] = start.z;
        //x += 3;
        if (start.x > border.maxX) border.maxX = start.x;
        if (start.y > border.maxY) border.maxY = start.y;
        if (start.x < border.minX) border.minX = start.x;
        if (start.y < border.minY) border.minY = start.y;
        vertices.push_back(start.x) ; vertices.push_back(start.y); vertices.push_back(0.0f);
        vertices.push_back(1);
        x += 3;
        pieces.first = start;
        
        idx.push_back(0); idx.push_back(1); last = 1;
       
        /* 
        vertices = x , y , z , draw?
        */
        
       
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4* sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);



        highlight.push_back(vertices[0]);  highlight.push_back(vertices[1]);  highlight.push_back(vertices[2]);
        hIdx.push_back(0); hIdx.push_back(1); colLast = 1;
       // hShader.use();
        glGenVertexArrays(1, &hVAO);
        glGenBuffers(1, &hVBO);
        glGenBuffers(1, &hEBO);
        
        glBindVertexArray(this->hVAO);
        glBindBuffer(GL_ARRAY_BUFFER, hVBO);
        glBufferData(GL_ARRAY_BUFFER, 4 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * idx.size(), &idx[0], GL_STATIC_DRAW);

       /* glBindBuffer(GL_ARRAY_BUFFER, hVBO);
        glBufferData(GL_ARRAY_BUFFER, 4 * highlight.size(), &highlight[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * hIdx.size(), &hIdx[0], GL_STATIC_DRAW);*/
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0); 
        glBindVertexArray(0);
	}

    
    float draw(Shader shader, Shader hShader, glm::vec3 end, float leftLength) {
        
        //vv[x] = end.x; vv[x + 1] = end.y; vv[x + 2] = end.z;
        //x += 3;
        /*int vv[6] = {
            0.0,0.0,0.0,
            end.x,end.y,0.0
        };*/
        //int xx[2] = { 0,1 };
        //std::cout << glm::distance(end, glm::vec3(vertices[vertices.size() - 3], vertices[vertices.size() - 2], vertices[vertices.size() - 1])) << std::endl;
        if (glm::distance(end, glm::vec3(vertices[vertices.size() - 4], vertices[vertices.size() - 3], vertices[vertices.size() - 2])) > smoothnes) {

            if (end.x > border.maxX) border.maxX = end.x;
            if (end.y > border.maxY) border.maxY = end.y;
            if (end.x < border.minX) border.minX = end.x;
            if (end.y < border.minY) border.minY = end.y;

            float tmp = glm::distance(glm::vec3(vertices[vertices.size() - 4], vertices[vertices.size() - 3], 0.0f), end);
            std::cout << tmp << std::endl;
            if (tmp <= leftLength) {
                length += tmp;
                leftLength -= tmp;
                this->vertices.push_back(vertices[vertices.size() - 4]); this->vertices.push_back(vertices[vertices.size() - 4]); this->vertices.push_back(0.0f);

                vertices.push_back(1);
                this->vertices.push_back(end.x); this->vertices.push_back(end.y); this->vertices.push_back(0.0f);

                vertices.push_back(1);
                //x += 3;
                idx.push_back(last); idx.push_back(last + 1);
                last += 2;
            }
            
        }
        return leftLength;
      
	}

    void draw(Shader shader,Shader hShader) {
        shader.use();
        shader.setVec3("color",color);
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
        glLineWidth(thin);
        //glDrawArrays(GL_LINES, 0, vertices.size()/3 - 1);
        glDrawElements(GL_LINES, idx.size()-1, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); 

        if (highlight.size() != 0) {
            shader.use();
            //shader.setVec3("color", glm::vec3(0.3, 0.8, 0.3));

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            //int iix[2] = { 0,20 };
            //int x = 5;
            //std::cout << highlight.size() << std::endl;
            /*glBufferData(GL_ARRAY_BUFFER, 4 * highlight.size(), &highlight[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hEBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * hIdx.size(), &hIdx[0], GL_STATIC_DRAW);*/
            glBufferData(GL_ARRAY_BUFFER, 4 * vertices.size(), &vertices[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * idx.size(), &idx[0], GL_STATIC_DRAW);

            glBindVertexArray(VAO);
            glLineWidth(1.5 * thin);
           // glDrawArrays(GL_LINES, 0, hIdx.size() - 1);
            glDrawElements(GL_LINES, idx.size()-1, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

    }

    void candidate(float x,float y) {
        int tmp = checkPiece(glm::vec3(x, y, 0));
        if (tmp == -1) return;
        if (vertices[tmp + 3] == 0.5 && vertices[tmp + 7] == 0.5) return;
        /*if (vertices[tmp + 3] == 0.5 && vertices[tmp + 7] == 0.5) {
            vertices[tmp + 3] = 1; vertices[tmp + 7] = 1;
        }*/
        else {
            vertices[tmp + 3] = 0.5; vertices[tmp + 7] = 0.5;
        }
        std::cout << "1 " <<vertices[tmp] << " " << vertices[tmp + 1] << " " << vertices[tmp + 2] << std::endl;
        std::cout << "2 " << vertices[tmp+4] << " " << vertices[tmp + 5] << " " << vertices[tmp + 6] << std::endl;
        //hghlight.push_back(vertices[tmp]);  highlight.push_back(vertices[tmp+1]);  highlight.push_back(vertices[tmp+2]);
        //highlight.push_back(vertices[tmp+4]);  highlight.push_back(vertices[tmp + 5]);  highlight.push_back(vertices[tmp + 6]);
        hIdx.push_back(colLast); hIdx.push_back(colLast + 1);
        colLast++;
    }

    void erase() {
        for (int i = 3; i < vertices.size(); i += 4) {
            if (vertices[i] == 0.5) vertices[i] = 0;
       }
    }

    private:
        
        int checkPiece(glm::vec3 position) {
            for (int j = 0; j <vertices.size() - 4; j += 4) {

                float slope = (vertices[j + 1] - vertices[j + 5]) / (vertices[j] - vertices[j + 4]);
                glm::vec3 tmp = glm::vec3(position.x + 1.0f, position.y + (-1 / slope), position.z);
                glm::vec3 start = glm::vec3(vertices[j], vertices[j + 1], vertices[j + 2]);
                glm::vec3 end = glm::vec3(vertices[j + 4], vertices[j + 5], vertices[j + 6]);
                auto re = lineIntersection(start, end, position, tmp);

                float angleWood = glm::degrees(glm::atan(slope));
                if ((re.x != FLT_MAX || re.y != FLT_MAX) && glm::distance(re, position) < 0.01) {
                    //std::cout << j << std::endl;
                    return j;
                }

            }
            return -1;

        }

};
