#include <format>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <thread>
#include "shader.h"

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window, Shader &shader);

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


    // Shaders
    Shader ourShader(
            R"(D:\CLionProjects\TamiOpenGL\shaders\tri.glsl)",
            R"(D:\CLionProjects\TamiOpenGL\shaders\tri.frag)"
            );

    // -------- VERTEX INPUT
    // Define vertices

    float vertices[1][18] = {
            {
                -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // left
                0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // right
                0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f  // top
            }
    };

    // Create a VERTEX BUFFER OBJECT for GPU memory management
    // Create a Vertex Array Object (VAO)
    // Create an Element Buffer Object (EBO)
    unsigned int EBO;
    unsigned int VBOs[1];
    unsigned int VAOs[1];

    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, VBOs);
    glGenBuffers(1, &EBO);

    for (int i = 0; i < std::size(vertices); i++) {
        // Bind VAOs
        glBindVertexArray(VAOs[i]);
        // Bind the buffer to GL_ARRAY_BUFFER
        // Than copy vertices into the VBO that has been bound to GL_ARRAY_BUFFER
        // GL_STATIC_DRAW defines that the data is set once and used many times
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
        // Tell OpenGL how the vertex data should be interpreted
        // - Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(0);
        // - Colour attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // Framerate/Frametiming
    double t_0 = glfwGetTime();
    double t_1;
    std::string title;
    double frametime;
    double framerate;

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Frametiming
        t_1 = glfwGetTime();

        frametime = t_1 - t_0;
        framerate = 1.0 / frametime;

        title = std::format(
                "TamiOpenGL, Frametime: {:.3f}ms, Framerate: {:.3f}FPS",
                frametime,
                framerate
        );

        t_0 = t_1;

        glfwSetWindowTitle(window, title.c_str());

        // Input
        processInput(window, ourShader);

        // Rendering logic
        glClearColor(0.2f, 0.2, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (unsigned int VAO: VAOs) {
            // Activate the program
            ourShader.use();
            // Configure Uniforms
            ourShader.setFloat("offset", 0.25f);
            // Draw our triangles
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        // Check and call events, afterward swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);

        // Scuffed as heck
        std::this_thread::sleep_for(std::chrono::milliseconds((100/(int)60)));
    }

    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);

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
}