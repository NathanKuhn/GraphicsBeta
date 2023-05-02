#include <stdio.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"

#include "ShaderLoading.h"
#include "renderer/Mesh.hpp"
#include "renderer/RenderObject.hpp"

#define ROT_SPEED 0.005f
#define MOUSE_CAP 10.0f
#define MOVE_SPEED 1.0f

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

    std::vector<glm::vec3> verts = {
        glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(-0.5f,-0.5f, 0.5f),
        glm::vec3( 0.5f, 0.5f, 0.5f),
        glm::vec3( 0.5f,-0.5f, 0.5f),
        glm::vec3(-0.5f, 0.5f,-0.5f),
        glm::vec3(-0.5f,-0.5f,-0.5f),
        glm::vec3( 0.5f, 0.5f,-0.5f),
        glm::vec3( 0.5f,-0.5f,-0.5f)
    };

    std::vector<glm::vec3> colors = {
        glm::vec3(0.0f, 1.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    };

    std::vector<glm::ivec3> indices = {
        glm::ivec3(0, 3, 2),
        glm::ivec3(0, 1, 3),
        glm::ivec3(2, 7, 6),
        glm::ivec3(2, 3, 7),
        glm::ivec3(6, 5, 4),
        glm::ivec3(6, 7, 5),
        glm::ivec3(4, 1, 0),
        glm::ivec3(4, 5, 1),
        glm::ivec3(0, 6, 4),
        glm::ivec3(0, 2, 6),
        glm::ivec3(1, 5, 7),
        glm::ivec3(1, 7, 3)
    };

    Renderer::Mesh cubeMesh = Renderer::Mesh(verts, colors, indices);
    Renderer::RenderObject cube = Renderer::RenderObject(cubeMesh);

    // Load the shader program

    GLuint programID = LoadShaders("SimpleVertex.vert", "SimpleFragment.frag");
    GLuint matrixID = glGetUniformLocation(programID, "Transform");

    int height, width;
    glfwGetWindowSize(window, &width, &height);
    float aspectRatio = (float)width / height;

    // Create transform matrix

    glm::mat4 projection = glm::perspective(glm::radians(80.0f), aspectRatio, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));

    glm::mat4 view;
    glm::mat4 mvp;

    float camera_rx = 0;
    float camera_ry = 0;
    float lastTime = glfwGetTime();
    float deltaTime = 0.0f;

    double mouse_px = 0;
    double mouse_py = 0;
    double mouse_x = 0;
    double mouse_y = 0;
    double mouse_dx = 0;
    double mouse_dy = 0;

    glfwGetCursorPos(window, &mouse_px, &mouse_py);

    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 0.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set up culling and depth

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        
        glfwGetCursorPos(window, &mouse_x, &mouse_y);
        
        mouse_dx = mouse_x - mouse_px;
        mouse_dy = mouse_y - mouse_py;

        mouse_px = mouse_x;
        mouse_py = mouse_y;

        camera_rx -= mouse_map(mouse_dy) * ROT_SPEED;
        camera_ry += mouse_map(mouse_dx) * ROT_SPEED;

        if (glfwGetKey(window, GLFW_KEY_W)) {
            camera_pos.z += glm::cos(camera_ry) * deltaTime * MOVE_SPEED;
            camera_pos.x += glm::sin(camera_ry) * deltaTime * MOVE_SPEED;
        }
        else if (glfwGetKey(window, GLFW_KEY_S)) {
            camera_pos.z -= glm::cos(camera_ry) * deltaTime * MOVE_SPEED;
            camera_pos.x -= glm::sin(camera_ry) * deltaTime * MOVE_SPEED;
        }

        if (glfwGetKey(window, GLFW_KEY_D)) {
            camera_pos.x += glm::cos(camera_ry) * deltaTime * MOVE_SPEED;
            camera_pos.z -= glm::sin(camera_ry) * deltaTime * MOVE_SPEED;
        }
        else if (glfwGetKey(window, GLFW_KEY_A)) {
            camera_pos.x -= glm::cos(camera_ry) * deltaTime * MOVE_SPEED;
            camera_pos.z += glm::sin(camera_ry) * deltaTime * MOVE_SPEED;
        }

        view = glm::mat4(1.0f);
        view = glm::rotate(view, 3.1415f - camera_rx, glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, -camera_ry, glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, -camera_pos);
        
        glm::vec3 result = view * glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        mvp = projection * view * model;

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &mvp[0][0]);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glDrawElements(GL_TRIANGLES, cubeMesh.triangles.size() * 3, GL_UNSIGNED_INT, NULL);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

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
    //glDeleteBuffers(1, &vbo);
    //glDeleteBuffers(1, &color_vbo);

    glfwTerminate();
    return 0;
}