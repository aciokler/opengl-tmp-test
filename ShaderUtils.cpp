//
//  ShaderUtils.cpp
//  MordernOpenGl
//
//  Created by Abraham-mac on 1/12/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#include "ShaderUtils.h"


GLuint ShaderUtils::createShaderFromFile( const GLchar *path, GLenum shaderType)
{
    std::ifstream fin;
    fin.open(path);
    if ( !fin.is_open() )
    {
        fin.close();
        std::cout << "Error opening file: '" << path << "'!" << std::endl;
        return 0;
    }
    
    std::string source((std::istreambuf_iterator<GLchar>(fin)), std::istreambuf_iterator<GLchar>());
    
    std::cout << source << std::endl;
    
    fin.close();
    
    GLuint shaderID = glCreateShader(shaderType);
    
    const GLchar *shaderSource = source.c_str();
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    
    glCompileShader(shaderID);
    
    GLint compileStatus;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);
    if ( compileStatus != GL_TRUE )
    {
        std::cout << "Failed to compile shader: '" << path << "'!" << std::endl;
        
        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength + 1, NULL, infoLog);
        std::cout << infoLog << std::endl;
        delete[] infoLog;
        
        glDeleteShader(shaderID);
        
        return 0;
    }
    
    return shaderID;
}

std::string ShaderUtils::getFileContents( const GLchar *path )
{
    std::ifstream fin;
    fin.open(path);
    if ( !fin.is_open() )
    {
        fin.close();
        std::cout << "Error opening file: '" << path << "'!" << std::endl;
        return 0;
    }
    std::string source((std::istreambuf_iterator<GLchar>(fin)), std::istreambuf_iterator<GLchar>());
    
    std::cout << source << std::endl;
    
    return source;
}
