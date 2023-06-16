#include <format>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <thread>
#include "include/stb_image.h"
#include "shader.h"

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window, Shader &shader, float &d);

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

    float vertices[] = {
            // positions          // colors           // texture1 coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    // - Colour attribute       R, G, B
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // - TexCoords attribute    S, T
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 *sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Load image
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(R"(D:\CLionProjects\TamiOpenGL\container.jpg)", &width, &height, &nrChannels, 0);
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
    data = stbi_load(R"(D:\CLionProjects\TamiOpenGL\awesomeface.png)", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture2" << std::endl;
    }
    stbi_image_free(data);

    // -------- RENDERING
    // Uniform assignment
    float mixValue = 0.2f;
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setFloat("mixValue", mixValue);

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
        processInput(window, ourShader, mixValue);

        // Rendering logic
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind textures to texture1 units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // Activate the program
        ourShader.use();
        ourShader.setFloat("mixValue", mixValue);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Check and call events, afterward swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Scuffed as heck
        std::this_thread::sleep_for(std::chrono::milliseconds((100/(int)60)));
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
void processInput(GLFWwindow *window, Shader &shader, float &d) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        shader.update();
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        d += 0.1f;
        if (d > 1.0f)
            d = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        d -= 0.1f;
        if (d < 0.0f)
            d = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}