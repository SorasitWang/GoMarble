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

#include "../header/Wood.h"
#include "../header/Marble.h"
#include "../header/Bin.h"
#include "../header/Menu.h"
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window, float deltatIme, Shader boxShader);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float xPos, yPos;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

bool adding = false;
Marble m = Marble();
Bin bin = Bin();

std::vector<Wood> map;
glm::vec3 start, end;
std::vector<Marble> marbles;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    //glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("./header/wood.vs", "./header/wood.fs"); // you can name your shader files however you like
    Shader marbleShader("./header/marble.vs", "./header/marble.fs");
    Shader iconShader("./header/icon.vs", "./header/icon.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    Menu menu = Menu();
    menu.init(ourShader,iconShader);
    bin.init(marbleShader);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);
    
   
    //m.init(marbleShader, glm::vec3(0.0f));
    //marbles.push_back(m);
    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        bin.draw(marbleShader,deltaTime);
        menu.draw(ourShader,iconShader);
       //w.draw(ourShader);
       for (auto &w : map)
           w.draw(ourShader);
       for (auto &mm : marbles) {
           //std::cout << mm.position.y << std::endl;
           mm.draw(marbleShader, deltaTime, map,bin);
       }
           //std::cout << m.velocity.y << std::endl;
       
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    xPos = xpos; yPos = ypos;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    Shader shader = Shader("./header/wood.vs", "./header/wood.fs");
    lastX = xpos;
    lastY = ypos;
    if (adding == true) {
   
        end = glm::vec3(2 * xPos / SCR_WIDTH - 1, 2 * (-yPos / SCR_HEIGHT + 0.5), 0.0f);
        //std::cout << end.x << std::endl;
        map[map.size() - 1].draw(shader, end);
    }
   
   
   // cam.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Shader shader = Shader("./header/wood.vs", "./header/wood.fs");
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            start = glm::vec3(2 * xPos / SCR_WIDTH - 1, 2 * (-yPos / SCR_HEIGHT + 0.5), 0.0f);
            if (adding == false) {
                //std::cout << "first" << std::endl;
                adding = true;
                map.push_back(Wood());
                map[map.size()-1].init(shader, start);
            }
           
         
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        adding = false;
        end = glm::vec3(2 * xPos / SCR_WIDTH - 1, 2 * (-yPos / SCR_HEIGHT + 0.5), 0.0f);
        
        map[map.size() - 1].draw(shader,end);
        //std::cout << map[map.size() - 1].vertices.size() << " " << map[map.size() - 1].idx.size() << std::endl;
      
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        Shader marbleShader("./header/marble.vs", "./header/wood.fs");
        marbles.push_back(Marble());
        marbles[marbles.size()-1].init(marbleShader, glm::vec3(2 * xPos / SCR_WIDTH - 1, 2 * (-yPos / SCR_HEIGHT + 0.5), 0.0f));
        marbles[marbles.size() - 1].draw(marbleShader, deltaTime, map,bin);
    }
}
