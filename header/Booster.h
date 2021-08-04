

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

struct Rocket {
    glm::vec3 position;
    bool avaliable = true;
};
class Booster {


public:
    Booster() {

    };
    float sizeX=0.15, sizeY=0.15;
    unsigned int VAO, VBO, EBO, texture;
    std::vector<Rocket> rockets;
    int amount = 3;
    float magnitude = 3.0;

    void init(Shader shader) {

        shader.use();

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
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        //เชื่อม

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vIcon), vIcon, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

       

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on currently bound texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char* data = stbi_load("C:\\Users\\LEGION\\source\\repos\\Marble\\res\\rocket.png", &width, &height, &nrChannels, 0);
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

    }
    
    void add(glm::vec3 pos) {
        if (amount <= 0) return;
        amount -= 1;
        rockets.push_back(Rocket());
        rockets[rockets.size() - 1].position = pos;
    }

    void draw(Shader shader) {
        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        for (auto& r : rockets) {
            if (!r.avaliable) continue;
            model = glm::translate(model, r.position);
            model = glm::scale(model, glm::vec3(0.3));
            shader.setMat4("model", model);

            

            glBindVertexArray(this->VAO);
            glBindTexture(GL_TEXTURE_2D, texture);
            //glDrawArrays(GL_LINES, 0, vertices.size()/3 - 1);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

        }

    }

    bool isCol(glm::vec3 pos,float rad) {
        for (int i = 0; i < rockets.size(); i++) {
            if (!rockets[i].avaliable) continue;
            if (pos.x +rad > rockets[i].position.x - sizeX && pos.x - rad < rockets[i].position.x + sizeX
                && pos.y + rad > rockets[i].position.y - sizeY && pos.y - rad < rockets[i].position.y + sizeY) {
                rockets[i].avaliable = false;
                return true;
            }
        }
        return false;

    }
};

    