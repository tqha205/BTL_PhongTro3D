#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Shader.h"
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

// Shaders for crosshair
const char* crosshairVertSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* crosshairFragSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 0.8);
}
)";

// Settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Camera state (First Person)
glm::vec3 camPos(0.0f, 1.7f, 5.0f);
glm::vec3 camFront(0.0f, 0.0f, -1.0f);
glm::vec3 camUp(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool autoRotate = false; // Auto-rotate camera feature
bool isCursorVisible = false; // Cursor visibility toggle
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (isCursorVisible) return; // Skip camera rotation when cursor is visible

    static float lastX = SCR_WIDTH / 2.0f;
    static float lastY = SCR_HEIGHT / 2.0f;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(front);
}

void processInput(GLFWwindow* window, Scene& scene, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 2.5f * deltaTime;
    glm::vec3 nextPos = camPos;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        nextPos += cameraSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        nextPos -= cameraSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        nextPos -= glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        nextPos += glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed;

    // Fly freely (removed height and wall clamps)
    camPos = nextPos;

    float rotateSpeed = 90.0f * deltaTime; // 90 degrees per second
    bool rotated = false;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        yaw -= rotateSpeed;
        rotated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        yaw += rotateSpeed;
        rotated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        pitch += rotateSpeed;
        rotated = true;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        pitch -= rotateSpeed;
        rotated = true;
    }

    if (rotated) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camFront = glm::normalize(front);
    }

    static bool lPressed = false;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && !lPressed) {
        scene.ToggleLights();
        lPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) lPressed = false;

    static bool clickPressed = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !clickPressed) {
        scene.ToggleDoor();
        clickPressed = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) clickPressed = false;

    // Auto-rotate toggle
    static bool rPressed = false;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rPressed) {
        autoRotate = !autoRotate;
        rPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) rPressed = false;

    // Toggle cursor visibility with TAB or C
    static bool togglePressed = false;
    if ((glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) && !togglePressed) {
        isCursorVisible = !isCursorVisible;
        if (isCursorVisible) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true; // reset for mouse_callback to avoid camera jump
        }
        togglePressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) togglePressed = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Arch-Viz Loft Simulation", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    // Crucial for 360 degree infinite camera rotation without jitter
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    Scene scene;
    Shader shader("shaders/pbr_vert.glsl", "shaders/pbr_frag.glsl");
    scene.Initialize();

    // Setup Crosshair
    float crosshairVertices[] = {
        -0.015f,  0.0f,
         0.015f,  0.0f,
         0.0f,  -0.025f,
         0.0f,   0.025f
    };
    unsigned int crosshairVAO, crosshairVBO;
    glGenVertexArrays(1, &crosshairVAO);
    glGenBuffers(1, &crosshairVBO);
    glBindVertexArray(crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &crosshairVertSource, NULL);
    glCompileShader(vShader);
    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &crosshairFragSource, NULL);
    glCompileShader(fShader);
    unsigned int crosshairProgram = glCreateProgram();
    glAttachShader(crosshairProgram, vShader);
    glAttachShader(crosshairProgram, fShader);
    glLinkProgram(crosshairProgram);

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, scene, deltaTime);
        scene.Update(deltaTime);

        if (autoRotate) {
            yaw += 15.0f * deltaTime; // Rotate 15 degrees per second
            if (yaw > 360.0f) yaw -= 360.0f;
            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            camFront = glm::normalize(front);
        }

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        float aspect = (float)width / (float)height;
        
        glClearColor(0.4f, 0.7f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        shader.use();
        
        float intensity = scene.isLightOn ? 50.0f : 0.0f;
        glm::vec3 lCol(intensity);
        glm::vec3 lPos[] = { glm::vec3(-5,8,-7), glm::vec3(5,8,-7), glm::vec3(-5,8,7), glm::vec3(5,8,7) };
        for(int i=0; i<4; i++) {
            shader.setVec3("lightPositions[" + std::to_string(i) + "]", lPos[i]);
            shader.setVec3("lightColors[" + std::to_string(i) + "]", lCol);
        }

        glm::mat4 projection = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("camPos", camPos);

        scene.DrawAll(shader);

#ifndef GL_LINES
#define GL_LINES 0x0001
#endif

        // Draw crosshair
        if (!isCursorVisible) {
            glUseProgram(crosshairProgram);
            glBindVertexArray(crosshairVAO);
            glClear(GL_DEPTH_BUFFER_BIT); // Clear depth instead of glDisable(GL_DEPTH_TEST)
            glDrawArrays(GL_LINES, 0, 4);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
