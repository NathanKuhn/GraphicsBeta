#include <stdio.h>
#include <iostream>
#include <random>
#include <chrono>

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
#include "renderer/Camera.hpp"
#include "renderer/WorldRenderer.hpp"

#define ROT_SPEED 0.005f
#define MOUSE_CAP 100.0f
#define MOVE_SPEED 10.0f

float move_fac = 1.0f;

void callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        move_fac *= 1.5;
    } else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        move_fac /= 1.5f;
    }
}

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

    /* Load the shader program for chunks. */
    Renderer::ShaderProgram chunkShader = Renderer::ShaderProgram("resources/shaders/ChunkVertex.vert", "resources/shaders/ChunkFragment.frag");
    chunkShader.addUniform("uTransform");

    /* Load the shader program for the sky. */
    Renderer::ShaderProgram skyShader = Renderer::ShaderProgram("resources/shaders/SkyVertex.vert", "resources/shaders/SkyFragment.frag");
    skyShader.addUniform("uTransform");

    std::vector<glm::vec3> skyVerts = {
        glm::vec3(-1, -1, 0.99999),
        glm::vec3(1, -1, 0.99999),
        glm::vec3(1, 1, 0.99999),
        glm::vec3(-1, 1, 0.99999)
    };

    std::vector<glm::vec3> skyNormals = {
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1)
    };

    std::vector<glm::vec2> skyUvs = {
        glm::vec2(0, 0),
        glm::vec2(1, 0),
        glm::vec2(1, 1),
        glm::vec2(0, 1)
    };

    std::vector<glm::ivec3> skyTris = {
        glm::ivec3(0, 1, 2),
        glm::ivec3(0, 2, 3)
    };

    Renderer::MeshData skyMesh = Renderer::MeshData(skyVerts, skyNormals, skyUvs, skyTris);
    Renderer::RenderObject skyQuad = Renderer::RenderObject(skyMesh);

    // Generate the world

    Data::World world = Data::World(4, 1, 4);

    // Create Camera and WorldRenderer

    Renderer::Camera camera = Renderer::Camera(glm::vec3(0.0f, 30.0f, 0.0f), glm::radians(80.0f), aspectRatio);
    Renderer::WorldRenderer renderer = Renderer::WorldRenderer(world, camera, chunkShader);

    camera.ry = 3.1415f;

    // Loop constants and setup

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

    glfwSetKeyCallback(window, callback);

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

        camera.rx += mouse_map(mouse_dy) * ROT_SPEED;
        camera.ry += mouse_map(mouse_dx) * ROT_SPEED;

        if (glfwGetKey(window, GLFW_KEY_W)) {
            camera.position.z -= glm::cos(camera.ry) * deltaTime * MOVE_SPEED * move_fac;
            camera.position.x += glm::sin(camera.ry) * deltaTime * MOVE_SPEED * move_fac;
        }
        else if (glfwGetKey(window, GLFW_KEY_S)) {
            camera.position.z += glm::cos(camera.ry) * deltaTime * MOVE_SPEED * move_fac;
            camera.position.x -= glm::sin(camera.ry) * deltaTime * MOVE_SPEED * move_fac;
        }

        if (glfwGetKey(window, GLFW_KEY_D)) {
            camera.position.x += glm::cos(camera.ry) * deltaTime * MOVE_SPEED * move_fac;
            camera.position.z += glm::sin(camera.ry) * deltaTime * MOVE_SPEED * move_fac;
        }
        else if (glfwGetKey(window, GLFW_KEY_A)) {
            camera.position.x -= glm::cos(camera.ry) * deltaTime * MOVE_SPEED * move_fac;
            camera.position.z -= glm::sin(camera.ry) * deltaTime * MOVE_SPEED * move_fac;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE)) {
            camera.position.y += deltaTime * MOVE_SPEED * move_fac;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
            camera.position.y -= deltaTime * MOVE_SPEED * move_fac;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /* Draw Sky. */
        skyShader.enable();

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        skyShader.setUniform("uTransform", glm::inverse(camera.getProjectionMatrix() * camera.getRotationMatrix()));
        glBindVertexArray(skyQuad.getVAO());
        glDrawElements(GL_TRIANGLES, skyQuad.getNumTriangles() * 3, GL_UNSIGNED_INT, NULL);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        /* Draw chunk. */
        renderer.Render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            glfwSetWindowShouldClose(window, true);
        }
    }

    // cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    renderer.cleanup();

    glfwTerminate();
    return 0;
}