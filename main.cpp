#include <format>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <filesystem>

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

std::string loadShader(const std::string& filepath);

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "TamiOpenGL", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

    }
    glfwMakeContextCurrent(window);

    // Derive function pointers via GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialise GLAD" << std::endl;
        return -1;
    }

    // Define size of viewport and register callback
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Shaders
    std::string vertexShaderString = loadShader(R"(D:\CLionProjects\TamiOpenGL\shaders\tri.glsl)");
    std::string fragmentShaderString = loadShader(R"(D:\CLionProjects\TamiOpenGL\shaders\tri.frag)");
    std::string fragmentShaderStringYellow = loadShader(R"(D:\CLionProjects\TamiOpenGL\shaders\triYellow.frag)");
    const char* vertexShaderSource = vertexShaderString.c_str();
    const char* fragmentShaderSource[2] = {
            fragmentShaderString.c_str(),
            fragmentShaderStringYellow.c_str()
    };
    int  success;
    char infoLog[512];


    // -------- COMPILING A VERTEX SHADER
    // Create a shader object
    // GL_VERTEX_SHADER defined as we are creating a vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach shader source to the shader object, then compile
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // -------- COMPILING A FRAGMENT SHADER
    unsigned int fragmentShaders[2];

    for (int i = 0; i < std::size(fragmentShaders); i++) {
        fragmentShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);

        // Attach shader source to the shader object, then compile
        glShaderSource(fragmentShaders[i], 1, &fragmentShaderSource[i], NULL);
        glCompileShader(fragmentShaders[i]);

        glGetShaderiv(fragmentShaders[i], GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(fragmentShaders[i], 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    // -------- SHADER PROGRAM
    // Create a shader program and store the ID reference to the new program
    unsigned int shaderPrograms[2] = {
            glCreateProgram(),
            glCreateProgram()
    };

    for (int i = 0; i < std::size(shaderPrograms); i++) {
        // Attach our shaders to the program and then link it before compiling
        glAttachShader(shaderPrograms[i], vertexShader);
        glAttachShader(shaderPrograms[i], fragmentShaders[i]);

        glLinkProgram(shaderPrograms[i]);

        glGetProgramiv(shaderPrograms[i], GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderPrograms[i], 512, NULL, infoLog);
            std::cout << "ERROR::SHADER_PROGRAM::LINKER::LINK_FAILED\n" << infoLog << std::endl;
        }
    }

    // Delete the shaders not needed anymore
    glDeleteShader(vertexShader);
    for (unsigned int & fragmentShader : fragmentShaders) {
        glDeleteShader(fragmentShader);
    }

    // -------- VERTEX INPUT
    // Define vertices

    float vertices[2][9] = {
            {
                    -0.5f / 2.0f - 0.5, -0.5f, 0.0f, // left
                    0.5f / 2.0f - 0.5, -0.5f, 0.0f, // right
                    0.0f / 2.0f - 0.5, 0.75f, 0.0f,  // top
                },
            {
                    -0.5f / 2.0f +0.5, -0.5f, 0.0f, // left
                    0.5f / 2.0f +0.5, -0.5f, 0.0f, // right
                    0.0f / 2.0f +0.5,  0.75f, 0.0f,  // top
            }
    };

    // Create a VERTEX BUFFER OBJECT for GPU memory management
    // Create a Vertex Array Object (VAO)
    // Create an Element Buffer Object (EBO)
    unsigned int EBO;
    unsigned int VBOs[2];
    unsigned int VAOs[2];

    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(0);
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

        glfwSetWindowTitle(window, title.c_str());
        t_0 = t_1;

        // Input
        processInput(window);

        // Rendering logic
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < std::size(VAOs); i++) {
            // Activate the program
            glUseProgram(shaderPrograms[i]);
            // Draw our triangles
            glBindVertexArray(VAOs[i]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        // Check and call events, afterward swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    for (int i = 0; i < std::size(shaderPrograms); i++) {
        glDeleteProgram(shaderPrograms[i]);
    }

    glfwTerminate();

    return 0;
}

// Callback for viewport resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Input processing
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Load shader from file
std::string loadShader(const std::string& filepath) {
    std::ifstream file {filepath};
    std::stringstream buffer;

    buffer << file.rdbuf();
    std::string source = buffer.str();

    return source;
}