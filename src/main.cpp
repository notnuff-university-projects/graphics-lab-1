#include <iostream>
#include <bit>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <thread>
#include <ranges>
#include <string>
#include <vector>


#include "shaders/gl_vertexShader.h"
#include "shaders/gl_fragmentShader.h"
#include "TextRenderer.h"


inline const int WIN_WIDTH = 600;
inline const int WIN_HEIGHT = 600;


GLFWwindow* StartOpenGL() {
    assert( glfwInit() );

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

//    auto monitorsCount = 0;
//    GLFWmonitor** monitors = glfwGetMonitors(&monitorsCount);

    GLFWwindow* win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "bruh", nullptr, nullptr);

    assert(win);

    glfwMakeContextCurrent(win);

    assert(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress) );

    return win;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
}

void ProcessInput(GLFWwindow* window) {
    if( glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS ) {
        glfwSetWindowShouldClose(window, true);
    }
}


GLuint CompileVertexShader(const char* vertexShaderSource) {
    // here we compile our vertex shader
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // ~

    {    // here we check whether the compilation was OK
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // ~
    }
    return vertexShader;
}

GLuint CompileFragmentShader(const char* fragmentShaderSource) {
    // here we compile our fragment shader
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // ~

    {
        // here we check whether the compilation was OK
        int success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // ~
    }
    return fragmentShader;
}

GLuint CompileProgram(const char *vertexShaderCode, const char *fragmentShaderCode) {
    // here we link all shaders with shader program
    auto shaderProgram = glCreateProgram();

    auto vertexShader = CompileVertexShader(vertexShaderCode);
    auto fragmentShader = CompileFragmentShader(fragmentShaderCode);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    {
        // here we check whether the compilation was OK
        int success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            // ~
        }
    }
    // ~

    // here we tell the opengl to use our fresh-created program
    glUseProgram(shaderProgram);
    // !

    // here we delete the shaders we linked, as we don`t need them anymore,
    // `cause they are in our shader program already
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader);
    // ~
    return shaderProgram;
}

struct VBOandVAO {
    unsigned int VBO;
    unsigned int VAO;
};

VBOandVAO GenerateVBOandVAO(const std::vector<float>& vertices,  const std::vector<int>& indices) {
    // 0. generate(allocate) VAO and VBO and EBO
    unsigned int VBO, VAO; //Vertex Buffer Object

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    unsigned int EBO; // Element Buffer Object
    glGenBuffers(1, &EBO);

    // 1. bind VAO, so all next actions will be applied to current VAO

    glBindVertexArray(VAO);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // 2. bind and set VBO

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // here we transfer our array data to our VBO buffer
    // GL_ARRAY_BUFFER -> target buffer that we are going to use
    // GL_STATIC_DRAW -> tip for GPU how to use memory of VBO
    // see also other tips for GPU

    const auto vsize = sizeof(float) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, vsize, vertices.data(), GL_STATIC_DRAW);
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    // 2.1 bind and set EBO
    const auto isize = sizeof(int) * indices.size();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize, indices.data(), GL_STATIC_DRAW);


    // 3. configure Vertex Attributes

    // with this function, we say to opengl how to interpret vertices data
    // in this case, we want to configure the location attribute of vertex,
    // as in our shader program before we defined
    // location = 0, so location attribute in our program will have
    // index 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    // ~

    // enable vertex attribute we defined before for index 0
    glEnableVertexAttribArray(0);
    // ~
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


    // 4. clear VBO and VAO
    // VBO was bound to Vertex Attribute=0 (location)
    // as buffer object, with glVertexAttribPointer()
    // so this VBO is saved in VAO, and we can now unbind it from GL_ARRAY_BUFFER

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind now VAO, as we saved it and it no longer have to be active VAO
    glBindVertexArray(0);

    return {VBO, VAO};
}

void InitIcon(GLFWwindow* win) {
    // bruh, icon is not working on wayland
    GLFWimage icons[1];
    icons[0].pixels = stbi_load("./rsc/img/icon.png", &icons[0].width, &icons[0].height, 0, 4); //rgba channels
    glfwSetWindowIcon(win, 1, icons);
    stbi_image_free(icons[0].pixels);
}

unsigned int VAO, VBO;
int main() {
    auto win = StartOpenGL();

    glfwSetFramebufferSizeCallback(win, FramebufferSizeCallback);
    FramebufferSizeCallback(win, WIN_WIDTH, WIN_HEIGHT);

    TextRenderer textRender;
    textRender.Init();

    InitIcon(win);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader("./rsc/shaders/gl_textVertexShader.glsl", "./rsc/shaders/gl_textFragmentShader.glsl");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIN_WIDTH), 0.0f, static_cast<float>(WIN_HEIGHT));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while ( !glfwWindowShouldClose(win) ) {
        ProcessInput(win);

        // set background color
        glClearColor(0.05f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        textRender.RenderText(shader, "Yaroshenko", 100.0f, 500.0f, 1.0f, VAO, VBO);
        textRender.RenderText(shader, "Oleksandr", 100.0f, 400.0f, 1.0f, VAO, VBO);
        textRender.RenderText(shader, "IM-21", 100.0f, 300.0f, 1.0f, VAO, VBO);
        textRender.RenderText(shader, "Smartphone Vivo", 400.0f, 100.0f, 0.3f, VAO, VBO);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
}
