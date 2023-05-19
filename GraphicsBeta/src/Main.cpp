#include <stdio.h>
#include <iostream>
#include <random>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include "renderer/MeshData.hpp"
#include "renderer/ChunkMesh.hpp"
#include "renderer/RenderObject.hpp"
#include "renderer/ShaderProgram.hpp"

#define ROT_SPEED 0.005f
#define MOUSE_CAP 100.0f
#define MOVE_SPEED 10.0f

float mouse_map(float x) {
    if (x > MOUSE_CAP) {
        return MOUSE_CAP;
    }
    if (x < -MOUSE_CAP) {
        return -MOUSE_CAP;
    }
    return x;
}

int main(void) {
    GLFWwindow* window;

    glm::mat4 a();

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "OpenGL Context", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        return -2;

    /* Set to 0 to turn off vsync. */
    glfwSwapInterval(1);

    int height, width;
    glfwGetWindowSize(window, &width, &height);
    float aspectRatio = (float)width / height;

    /* Generate a single chunk. */
    std::default_random_engine gen;
    std::uniform_real_distribution<float> dist = std::uniform_real_distribution<float>(0.0f, 1.0f);
   
    unsigned short chunkData[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                unsigned short block;
                if (y < 3) {
                    block = 1;
                } else if (y == 3 && dist(gen) < 0.5f) {
                    block = 1;
                } else {
                    block = 0;
                }

                chunkData[x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z] = block;
            }
        }
    }

    Data::Chunk chunk = Data::Chunk(chunkData, glm::ivec3(0, 0, 0));

    Renderer::ChunkMesh chunkMesh = Renderer::ChunkMesh(chunk);

    /* Load the texture. */
    GLuint grassTexture;
    glGenTextures(1, &grassTexture);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int texWidth, texHeight, nrChannels;
    unsigned char* data = stbi_load("resources/textures/grass.png", &texWidth, &texHeight, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load image.\n";
    }
    stbi_image_free(data);

    /* Load the shader program. */
    Renderer::ShaderProgram chunkShader = Renderer::ShaderProgram("resources/shaders/SimpleVertex.vert", "resources/shaders/SimpleFragment.frag");
    chunkShader.AddUniform("uTransform");

    // Create transform matrix

    glm::mat4 projection = glm::perspective(glm::radians(80.0f), aspectRatio, 0.1f, 100.0f);
    glm::mat4 view;
    glm::mat4 finalTransform;
    glm::vec3 camera_pos = glm::vec3(0.0f, 10.0f, 0.0f);

    float camera_rx = 0;
    float camera_ry = 0;
    float lastTime = glfwGetTime();
    float startTime = glfwGetTime();
    float deltaTime = 0.0f;
    float totalTime = 0.0f;

    double mouse_px = 0;
    double mouse_py = 0;
    double mouse_x = 0;
    double mouse_y = 0;
    double mouse_dx = 0;
    double mouse_dy = 0;

    /* Lock the cursor. */
    glfwGetCursorPos(window, &mouse_px, &mouse_py);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /* Set up culling and depth. */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    
    glClearColor(0.70, 0.87, 0.95, 1.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        totalTime = lastTime - startTime;
        
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        
        mouse_dx = mouse_x - mouse_px;
        mouse_dy = mouse_y - mouse_py;

        mouse_px = mouse_x;
        mouse_py = mouse_y;

        camera_rx += mouse_map(mouse_dy) * ROT_SPEED;
        camera_ry += mouse_map(mouse_dx) * ROT_SPEED;

        if (glfwGetKey(window, GLFW_KEY_W)) {
            camera_pos.z -= glm::cos(camera_ry) * deltaTime * MOVE_SPEED;
            camera_pos.x += glm::sin(camera_ry) * deltaTime * MOVE_SPEED;
        }
        else if (glfwGetKey(window, GLFW_KEY_S)) {
            camera_pos.z += glm::cos(camera_ry) * deltaTime * MOVE_SPEED;
            camera_pos.x -= glm::sin(camera_ry) * deltaTime * MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_D)) {
            camera_pos.x += glm::cos(camera_ry) * deltaTime * MOVE_SPEED;
            camera_pos.z += glm::sin(camera_ry) * deltaTime * MOVE_SPEED;
        }
        else if (glfwGetKey(window, GLFW_KEY_A)) {
            camera_pos.x -= glm::cos(camera_ry) * deltaTime * MOVE_SPEED;
            camera_pos.z -= glm::sin(camera_ry) * deltaTime * MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE)) {
            camera_pos.y += deltaTime * MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            camera_pos.y -= deltaTime * MOVE_SPEED;
        }

        view = glm::mat4(1.0f);
        view = glm::rotate(view, camera_rx, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, camera_ry, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, -camera_pos);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        chunkShader.Enable();

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        /* Draw chunk. */

        Renderer::RenderObject chunkObject = chunkMesh.getRenderObject();
        finalTransform = projection * view * chunkObject.getTransform();
        chunkShader.SetUniform("uTransform", finalTransform);
        glBindBuffer(GL_ARRAY_BUFFER, chunkObject.getVAO());
        //glBindVertexArray(chunkObject.getVAO());
        //glDrawArrays(GL_TRIANGLES, 0, chunkObject.getNumVerts());
        glDrawElements(GL_TRIANGLES, chunkObject.getNumTriangles() * 3, GL_UNSIGNED_INT, NULL);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    // cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    chunkMesh.getRenderObject().cleanUp();

    glfwTerminate();
    return 0;
}