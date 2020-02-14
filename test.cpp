
#define GLFW_INCLUDE_GLCOREARB

#include <cmath>

#include "ShaderUtils.h"
#include "Program.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iL/il.h>

//tdogl::Program *program = NULL;

enum
{
    POSITIONS_BUFF,
    INDEX_BUFF,
    COLOR_BUFF,
    TOTAL_BUFFERS
};

int main(int argc, const char * argv[])
{
    GLint glfwStatus = glfwInit();
    if ( glfwStatus != GL_TRUE )
    {
        std::cout << "glfw failed! :(" << std::endl;
        return 0;
    }
    
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    
    GLFWwindow* window = glfwCreateWindow(640, 360, "Modern Open GL", NULL, NULL);
    if (!window)
    {
        std::cout << "failed to create window! :(" << std::endl;
        glfwTerminate();
        return 0;
    }
    
    glfwMakeContextCurrent(window);
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    GLuint vertexShader = ShaderUtils::createShaderFromFile("myShaderProgram.vs", GL_VERTEX_SHADER);
    GLuint fragmentShader = ShaderUtils::createShaderFromFile("myShaderProgram.fs", GL_FRAGMENT_SHADER);
    
    //GLuint vertexShader = tdogl::Shader::getShader("myShaderProgram.vs", GL_VERTEX_SHADER);
    //GLuint fragmentShader = tdogl::Shader::getShader("myShaderProgram.fs", GL_FRAGMENT_SHADER);
    
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    glBindFragDataLocation(shaderProgram, 0, "fragData");
    //glBindAttribLocation(shaderProgram, 0, "position");
    
    glLinkProgram(shaderProgram);
    
    // detach the shaders from the program
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    
    GLint linkStatus;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
    if ( linkStatus != GL_TRUE )
    {
        std::cout << "could not link shader program!" << std::endl;
        GLint infoLogLength;
        
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar * linkError = new GLchar[ infoLogLength + 1 ];
        glGetProgramInfoLog(shaderProgram, infoLogLength + 1, NULL, linkError);
        
        std::cout << linkError << std::endl;
        
        delete [] linkError;
        
        glDeleteProgram(shaderProgram);
        
        return 0;
    }
    
    
//    tdogl::Program *program = new tdogl::Program();
//    program->attachShader(&vertexShader);
//    program->attachShader(&fragmentShader);
//    
//    program->link();
//    
//    program->detachShader(&vertexShader);
//    program->detachShader(&fragmentShader);
    
    GLuint myVBO;
    GLuint * vbos = new GLuint[3];
    glGenBuffers(3, vbos);
//    glGenBuffers(1, &myVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[ POSITIONS_BUFF ] );
    
    GLfloat bufferData[] =
    {
       //  0.5, 0.5, 0.0,
       // -0.5, 0.5, 0.0,
       // -0.5,-0.5, 0.0,
       //  0.5,-0.5, 0.0
        
        // front
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, NULL);
    
    GLuint myVAO;
    GLuint * vaos = new GLuint[1];
//    glGenVertexArrays(1, &myVAO);
    glGenVertexArrays(1, vaos);
    glBindVertexArray( vaos[0] );
    
    GLint positionLoc = glGetAttribLocation(shaderProgram, "position");
    
    //GLint positionLoc = program->attrib("position");
    
    if ( positionLoc < 0 )
    {
        std::cout << "Failed to find the location pointer!" << std::endl;
        return 0;
    }
    
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    
    
    GLuint colorVBO;
//    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[COLOR_BUFF]);
    
    GLfloat cube_colors[] = {
        // front colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        // back colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
    
    GLint vertexColorLoc = glGetAttribLocation(shaderProgram, "v_color");
    
    glEnableVertexAttribArray(vertexColorLoc);
    glVertexAttribPointer(vertexColorLoc, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);
    
    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindVertexArray(NULL);
    
    
    GLuint myEBO;
//    glGenBuffers(1, &myEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[INDEX_BUFF] );
    
    GLushort indicesData[] =
    {
        //0, 1, 2,
        //0, 2, 3
        
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        3, 2, 6,
        6, 7, 3,
        // back
        7, 6, 5,
        5, 4, 7,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // left
        4, 0, 3,
        3, 7, 4,
        // right
        1, 5, 6,
        6, 2, 1,
    };
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesData), indicesData, GL_STATIC_DRAW);
    
    GLint colorLoc = glGetUniformLocation(shaderProgram, "color");
    GLint uniform_m_transform = glGetUniformLocation(shaderProgram, "m_transform");
    
    while ( !glfwWindowShouldClose(window) )
    {
        GLint viewPortWidth, viewPortHeight;
        glfwGetFramebufferSize(window, &viewPortWidth, &viewPortHeight);
        glViewport(0, 0, viewPortWidth, viewPortHeight);
        // clear color and depth...
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        // rotation calculation...
        float move = sinf(glfwGetTime() * (4*3.14) / 5); // -1<->+1 every 5 seconds
        float angle = glfwGetTime() * 45;  // 45° per second
        
        glm::vec3 axis_y(0.0, 1.0, 0.0);
        glm::mat4 anim = glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), angle, axis_y);
        
        //glm::vec3 axis_z(0, 0, 1);
        
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
        glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 projection = glm::perspective(45.0f, 1.0f*viewPortWidth/viewPortHeight, 0.1f, 10.0f);
        
        //glm::mat4 m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(move, 0.0, 0.0)) * glm::rotate(glm::mat4(1.0f), angle, axis_z);
        
        glm::mat4 m_transform = projection * view * model * anim;
        
        //assigning the value to the vertex shader uniform matrix...
        glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
        
        glUniform4f(colorLoc, sinf(glfwGetTime()*2), 0.0, 1.0, 1.0);
        
        glBindVertexArray( vaos[0] );
        
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[INDEX_BUFF] );
        int size;
        glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        
        glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
        
        glBindVertexArray(NULL);
        
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    std::cout << "hello world!" << std::endl;
    
    glDeleteBuffers(1, &myVBO);
    glDeleteBuffers(1, &colorVBO);
    glDeleteVertexArrays(1, &myVAO);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    
    delete [] vbos;
    delete [] vaos;
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}
