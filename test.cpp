#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

static unsigned int compileShader( unsigned int type, const string& source )
{
    unsigned int id = glCreateShader( type );
    const char* src = source.c_str();
    glShaderSource( id, 1, &src, nullptr );
    glCompileShader( id );

    int result = 0;
    glGetShaderiv( id, GL_COMPILE_STATUS, &result );
    if( result == GL_FALSE )
    {
        int length = 0;
        glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );
        char* message = (char*)alloca( length * sizeof( char ) );
        glGetShaderInfoLog( id, length, &length, message );
        cout << "Failed to compile " << ( type == GL_VERTEX_SHADER ? "vertex" : "fragment" ) << "shader" << endl;
        cout << message << endl;
        glDeleteShader( id );
    }
    return id;
}

static int createShader( const string& vertexShader, const string& fragmentShader )
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader( GL_VERTEX_SHADER, vertexShader );
    unsigned int fs = compileShader( GL_FRAGMENT_SHADER, fragmentShader );

    glAttachShader( program, vs );
    glAttachShader( program, fs );
    glLinkProgram( program );
    glValidateProgram( program );

    glDeleteShader( vs );
    glDeleteShader( fs );

    return program;
}

int main( void )
{
    GLFWwindow* window;
    /* Initialize the library */
    if( !glfwInit() )
        return -1;

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow( 640, 480, "Hello World", NULL, NULL );
    if( !window )
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent( window );
    if( glewInit() != GLEW_OK )
        cout << "error" << endl;

    cout << glGetString( GL_VERSION ) << endl;

    GLuint vao = 0;
    glCreateVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout ( location = 0 ) in vec4 position;"
        "\n"
        "void main(){\n"
        "gl_Position = position;\n"
        "}\n";
    string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout ( location = 0 ) out vec4 color;"
        "\n"
        "void main(){\n"
        "color = vec4(1.0,1.0,0.0,1.0);\n"
        "}\n";
    unsigned int shader = createShader( vertexShader, fragmentShader );
    glUseProgram( shader );

    float positions[ 6 ] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         -1.0f,  1.0f,
    };
    unsigned int buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, 6 * sizeof( float ), positions, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 2, 0 );

    /* Loop until the user closes the window */
    while( !glfwWindowShouldClose( window ) ) {

        positions[5] = 1 - positions[5];
        /* Render here */
        glClear( GL_COLOR_BUFFER_BIT );

        glDrawArrays( GL_TRIANGLES, 0, 3 );

        /* Swap front and back buffers */
        glfwSwapBuffers( window );

        /* Poll for and process events */
        glfwPollEvents();
        std::cout << "adasdf" << std::endl;
    }

    glfwTerminate();
    return 0;
}