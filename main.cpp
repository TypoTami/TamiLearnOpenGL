#include <format>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <thread>   
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "include/stb_image.h"
#include "shader.h"

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow *window, Shader &shader);

// GLOBALS
// -- CAMERA
glm::vec3 cameraPos     = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront   = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp      = glm::vec3(0.0f, 1.0f, 0.0f);
float pitch = 0.0f, yaw = -90.0f, fov = 35.0f;
// -- MOUSE INPUT
float lastX = 400.0f, lastY = 300.0f;
bool firstMouse = true;
// -- DELTATIME
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Main
int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << std::filesystem::current_path() << std::endl;

    // Init GLFW and set hints
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create our window and define it our context
    GLFWwindow* window = glfwCreateWindow(800, 600, "TamiOpenGL", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Derive function pointers via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << std::format("Maximum number of vertex attributes supportred: {}", nrAttributes) << std::endl;

    // Define size of viewport and register callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Configure global OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Shaders
    Shader ourShader(
            R"(C:\Users\tombu\Documents\InteliJ\CLionProjects\TamiLearnOpenGL\shaders\tri.glsl)",
            R"(C:\Users\tombu\Documents\InteliJ\CLionProjects\TamiLearnOpenGL\shaders\tri.frag)"
            );

    // -------- VERTEX INPUT
    // Define vertices

    float vertices[] = {
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

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    // Create a VERTEX BUFFER OBJECT for GPU memory management
    // Create a Vertex Array Object (VAO)
    // Create an Element Buffer Object (EBO)
    unsigned int EBO;
    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    // Bind VAOs
    glBindVertexArray(VAO);
    // Bind the buffer to GL_ARRAY_BUFFER
    // Than copy vertices into the VBO that has been bound to GL_ARRAY_BUFFER
    // GL_STATIC_DRAW defines that the data is set once and used many times
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Bind EBOs
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Tell OpenGL how the vertex data should be interpreted
    // - Position attribute     X, Y, Z
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    // - TexCoords attribute    S, T
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 *sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // -------- TEXTURES
    unsigned int texture1, texture2;
    int width, height, nrChannels;
    unsigned char *data;

    // Generate texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set wrapping + filtering options on the current object bound to the GL_TEXTURE_2D target
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(R"(C:\Users\tombu\Documents\InteliJ\CLionProjects\TamiLearnOpenGL\container.jpg)", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data);

    // Generate texture2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set wrapping + filtering options on the current object bound to the GL_TEXTURE_2D target
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(R"(C:\Users\tombu\Documents\InteliJ\CLionProjects\TamiLearnOpenGL\pain.jpg)", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture2" << std::endl;
    }
    stbi_image_free(data);

    // -------- TRANSLATION
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // -------- RENDERING
    // Uniform assignment
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    // Framerate/Frametiming
    std::string title;
    double framerate;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Frametiming
        float currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        framerate = 1.0 / deltaTime;

        title = std::format(
                "TamiOpenGL, Frametime: {:.3f}ms, Framerate: {:.3f}FPS",
                deltaTime,
                framerate
        );
        lastFrame = currentFrame;

        glfwSetWindowTitle(window, title.c_str());

        // Update view matrix with Camera's parameters
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // We translate opposite of where we want to move (from the camera PoV)
        projection = glm::perspective(glm::radians(fov), 800.f / 600.f, 0.1f, 100.f);

        // Input
        processInput(window, ourShader);

        // Rendering logic
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind textures to texture1 units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Activate the program
        ourShader.use();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setFloat("mixVal", sinf((float)glfwGetTime() * 2.f) / 4.f + .5f);
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++) {
            // Update model matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;

            if (i%3 == 0) {
                model = glm::rotate(model, glm::radians(angle)+(float)glfwGetTime()*glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.3f));
            } else {

                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.3f));
            }

            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Check and call events, afterward swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Scuffed as heck
//        std::this_thread::sleep_for(std::chrono::milliseconds((100/(int)10)));
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();

    return 0;
}

// Callback for viewport resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Input processing
void processInput(GLFWwindow *window, Shader &shader) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        shader.update();
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calulate offsets
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    // Apply sensitivity
    const float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Apply offsets to yaw and pitch
    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    // Calculate and apply direction vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 70.0f)
        fov = 70.0f;
}