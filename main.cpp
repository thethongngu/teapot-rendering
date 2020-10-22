#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Point {
    double x, y, z;
    Point(double xx, double yy, double zz) {
        x = xx;
        y = yy; 
        z = zz;
    }
};

struct Face {
    double x, y, z;
    Face(double xx, double yy, double zz) {
        x = xx;
        y = yy; 
        z = zz;
    }
};

std::vector<Point> vertices;
std::vector<Face> faces;

void load_obj() {
    std::ifstream obj_file;
    obj_file.open("teapot.obj");

    std::string dummy;
    std::getline(obj_file, dummy);
    std::getline(obj_file, dummy);
    std::getline(obj_file, dummy);

    char type;
    while (!obj_file.eof()) {
        obj_file >> type;
        if (type == 'v') {
            double x, y, z;
            obj_file >> x >> y >> z;
            vertices.push_back(Point(x, y, z));

        } else {
            int x, y, z;
            obj_file >> x >> y >> z;
            faces.push_back(Face(x - 1, y - 1, z - 1));
        }
    }

    obj_file.close();
}

GLuint compile_shader(GLuint type, std::string path) {
    GLuint shader_id = glCreateShader(type);
    std::ifstream file(path);
    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const char* source_c_str = source.c_str();
    glShaderSource(shader_id, 1, &source_c_str, nullptr);
    glCompileShader(shader_id);

    int status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        int log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
        char* error = (char*) alloca(log_length * sizeof(char));

        glGetShaderInfoLog(shader_id, log_length, &log_length, error);
        std::cout << "Failed to compile " << ( type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << "shader" << std::endl;
        std::cout << error << std::endl;
        glDeleteShader(shader_id);
    }

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, shader_id);
    glLinkProgram(shader_program);
    glValidateProgram(shader_program);
    glDeleteShader(shader_id);

    return shader_program;
}

int main(void) {
    GLFWwindow* window;
    if (!glfwInit()) exit(EXIT_FAILURE);

    window = glfwCreateWindow(800, 600, "Project 01", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    GLuint my_shader = compile_shader();
    glUseProgram(my_shader);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 3 * size(double), )

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}