#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Point {
    float x, y, z;
    Point(float xx, float yy, float zz) {
        x = xx;
        y = yy; 
        z = zz;
    }
};

struct Face {
    float x, y, z;
    Face(float xx, float yy, float zz) {
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
            float x, y, z;
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

void compile_shader(GLuint type, std::string path, GLuint& shader_program) {
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

    glAttachShader(shader_program, shader_id);
    glLinkProgram(shader_program);
    glValidateProgram(shader_program);
    glDeleteShader(shader_id);
}

int main(void) {
    GLFWwindow* window;
    if (!glfwInit()) exit(EXIT_FAILURE);

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );

    window = glfwCreateWindow(800, 600, "Project 01", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glewInit();

    GLuint vao = 0;
    glCreateVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    
    GLuint shader_program = glCreateProgram();
    compile_shader(GL_VERTEX_SHADER, "vertex_shader.glsl", shader_program);
    compile_shader(GL_FRAGMENT_SHADER, "fragment_shader.glsl", shader_program);
    glUseProgram(shader_program);

    GLint model_shader = glGetUniformLocation(shader_program, "model");
    GLint view_shader = glGetUniformLocation(shader_program, "view");
    GLint projection_shader = glGetUniformLocation(shader_program, "projection");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(4,3,3),  glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(model_shader, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(view_shader, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projection_shader, 1, GL_FALSE, &projection[0][0]);

    float positions[ 6 ] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         -1.0f,  1.0f,
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}