#include "libs.h"

Vertex vertices[] = {
    //position                          //colour                        //texcoords
    glm::vec3(-0.5f, 0.5f, 0.f),        glm::vec3(1.f, 0.f, 0.f),       glm::vec2(0.f, 1.f),
    glm::vec3(-0.5f, -0.5f, 0.f),       glm::vec3(0.f, 1.f, 0.f),       glm::vec2(0.f, 0.f),
    glm::vec3(0.5f, -0.5f, 0.f),        glm::vec3(0.f, 0.f, 1.f),       glm::vec2(1.f, 0.f),
    glm::vec3(0.5f, 0.5f, 0.f),         glm::vec3(0.f, 1.f, 0.f),       glm::vec2(0.f, 0.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
    0, 1, 2, //triangle 1
    0, 2, 3 //triangle 2
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

void updateInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint &program) {
    bool loadSuccess = true;
    char infoLog[512];
    GLint success;

    std::string temp = "";
    std::string src = "";

    std::ifstream in_file;

    //vertex
    in_file.open("vertex_core.glsl");

    if(in_file.is_open()) {
        while(std::getline(in_file, temp)) {
            src += temp + "\n";
        }
    }
    else {
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
        loadSuccess = false;
    }

    in_file.close();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertSrc = src.c_str();
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false;
    }

    temp = "";
    src = "";

    //fragment
    in_file.open("fragment_core.glsl");

    if(in_file.is_open()) {
        while(std::getline(in_file, temp)) {
            src += temp + "\n";
        }
    }
    else {
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
        loadSuccess = false;
    }

    in_file.close();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragSrc = src.c_str();
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false;
    }

    //program
    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << "\n";
        std::cout << infoLog << "\n";
        loadSuccess = false;
    }

    //end
    glUseProgram(0);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return loadSuccess;
}

int main() {

    //init glfw
    glfwInit();

    //create window
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;
    int framebufferWidth = 0;
    int framebufferHeight = 0;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); MACOS

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Jiggle Launcher", NULL, NULL);

    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);

    //glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    //glViewport(0, 0, framebufferWidth, framebufferHeight);

    glfwMakeContextCurrent(window); //important

    //init glew
    glewExperimental = GL_TRUE;

    //error
    if(glewInit() != GLEW_OK) {
        std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
        glfwTerminate();
    }

    //opengl options
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //shader init
    GLuint core_program;
    if(!loadShaders(core_program)) {
        glfwTerminate();
    }

    //model

    //vao, vbo, ebo

    //gen vao and bind
    GLuint VAO;
    glCreateVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //gen vbo and bind and send data
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //gen ebo and bind and send data
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //set vertexattribpointers and enable (input assembly)
    //position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    //colour
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    //texcoord
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
    glEnableVertexAttribArray(2);

    //bind vao 0
    glBindVertexArray(0);

    //texture init
    int image_width = 0;
    int image_height = 0;
    unsigned char* image = SOIL_load_image("Textures/icon.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

    GLuint texture0;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    if(image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
    }

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    //main loop
    while(!glfwWindowShouldClose(window)) {
        //update input
        glfwPollEvents();

        //update
        updateInput(window);

        //draw

        //clear
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //use a program
        glUseProgram(core_program);

        //activate texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);

        //bind vertex array object
        glBindVertexArray(VAO);

        //draw
        //glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
        glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

        //end draw
        glfwSwapBuffers(window);
        glFlush();
    }

    //end of program
    glfwDestroyWindow(window);
    glfwTerminate();

    //delete program
    glDeleteProgram(core_program);

    return 0;
}