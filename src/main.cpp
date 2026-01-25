#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cmath>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderLoader.h"
#include "TextureLoader.h"

float gArrow = 0.2f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime = 0.f;
float lastFrame = 0.f;
float yaw = -90.f;
float pitch = 0.f;
float lastX = 400, lastY = 350;
bool firstMouse = true;
float Zoom = 40;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y ranges bottom to top
    lastX = xpos;
    lastY = ypos;
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        gArrow += 0.0005f;
        if (gArrow > 1.0f)
        {
            gArrow = 1.0f;
        }
        if (gArrow < 0.f) {
            gArrow = 0.f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        gArrow -= 0.0005f;
        if (gArrow > 1.0f)
        {
            gArrow = 1.0f;
        }
        if (gArrow < 0.f) {
            gArrow = 0.f;
        }
    }

    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
        cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
        cameraSpeed;
}



int main(void)
{
    GLFWwindow* window;
    int windowWidth = 800;
    int windowHeight = 600;
    float vertices[] = {
	 -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)); // convert to radians first
    direction.z = sin(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    //float rectangle[] = {
    // 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
    // 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// bottom right
    //-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,// bottom left
    //-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
    //};

    float cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f

    };

    unsigned int indicies[] = {
        0, 1, 3,
        1, 2, 3
    };

    glm::vec3 cubePositions[] = {
        glm::vec3(0.20f, 0.30f, 0.40f),
        glm::vec3(2.0f, 5.0f,-15.0f),
        glm::vec3(-1.5f,-2.2f,-2.5f),
        glm::vec3(-3.8f,-2.0f,-12.3f),
        glm::vec3(2.4f,-0.4f,-3.5f),
        glm::vec3(-1.7f, 3.0f,-7.5f),
        glm::vec3(1.3f,-2.0f,-2.5f),
        glm::vec3(1.5f, 2.0f,-2.5f),
        glm::vec3(1.5f, 0.2f,-1.5f),
        glm::vec3(-1.3f, 1.0f,-1.5f)
    };

    


   /* glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(glm::vec3(0.f, 0.f, 3.f),
                       glm::vec3(0.f, 0.f, 0.f),
                       glm::vec3(0.f, 1.f, 0.f));*/
    
    /* Initialize the library */
    if (!glfwInit())
    {
        printf("Crash at glfw init\n");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "if you read this, you are gae", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Crash at window\n");
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Calculate centered position
    
	printf("Screen Width: %d, Height: %d\n", mode->width, mode->height);

    int xpos = (mode->width - windowWidth) / 2;
    int ypos = (mode->height - windowHeight) / 2;

    // Set position
    glfwSetWindowPos(window, xpos, ypos);
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }
    glViewport(0, 0, windowWidth, windowHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    Texture img = Texture("../assets/container.jpg");
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), img.getHeight(), 0, GL_RGB,
        GL_UNSIGNED_BYTE, img.getData());
    glGenerateMipmap(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);
    Texture img2 = Texture("../assets/yz.jpg");
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img2.getWidth(), img2.getHeight(), 0, GL_RGB,
        GL_UNSIGNED_BYTE, img2.getData());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);


    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    shaderLoader shader1("../shaders/vertex.glsl", "../shaders/fragment.glsl");
    shader1.createShaders(ShaderCreationMode::BOTH_FROM_FILE);
    shader1.useProgram();
    shader1.setInt("texture2", 1);
    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    /*unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);*/


    glBindVertexArray(0);
    float temp = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Convert mouse coordinates to 0.0 - 1.0 range (Normalized Device Coordinates)
        // Note: mouse Y is top-down in GLFW, so we flip it
        float normMouseX = (float)mouseX / windowWidth;
        float normMouseY = 1.0f - ((float)mouseY / windowHeight);
        
        /*glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.f), 
            glm::vec3(0.5f, 1.0f, 0.0f));*/

        /*const float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;*/
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(Zoom), (float)(800 / 700), 0.1f, 100.f);

        // Convert pixel coordinates (0 to 800) to OpenGL coordinates (-1 to 1)
        float openglX = (xpos / (800.0f / 2.0f)) - 1.0f;
        float openglY = 1.0f - (ypos / (600.0f / 2.0f));

        glClearColor(.2f, .3f, .3f, 1.f);
        //glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//shader1.setFloat("xOffset", 0.5f);
        /*float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = shader1.getUniformLocation("ourColor");
        if (vertexColorLocation == -1) {
            printf("Location not found\n");
        }
        
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/
        /* Render here */

        /*shader1.useProgram();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);*/
        
        
        shader1.setMat4("projection", projection);
        shader1.setMat4("view", view);
        shader1.setVec2("mousePos", normMouseX, normMouseY);
        shader1.setFloat("rate", gArrow);
        
        
        glBindVertexArray(VAO2);
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            if(i == 0 || i % 3 == 0)
            {
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.f * i * cos(glfwGetTime());
                if (i == 0) {
                    angle = 20.f * 1 * sin(glfwGetTime());
                }
                model = glm::rotate(model, glm::radians(angle),
                    glm::vec3(1.0f, 0.3f, 0.5f));
                shader1.setMat4("model", model);
                shader1.useProgram();
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            else {
                model = glm::translate(model, cubePositions[i]);
                shader1.setMat4("model", model);
                shader1.useProgram();
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        /*shader1.useProgram();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);*/

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}