//
//  ShaderUtils.h
//  MordernOpenGl
//
//  Created by Abraham-mac on 1/12/14.
//  Copyright (c) 2014 Abraham-mac. All rights reserved.
//

#ifndef __MordernOpenGl__ShaderUtils__
#define __MordernOpenGl__ShaderUtils__

#define GLFW_INCLUDE_GLCOREARB

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>

#include <GLFW/glfw3.h>


class ShaderUtils
{
public:
    static GLuint createShaderFromFile( const GLchar *path, GLenum shadaerType);
    
    static std::string getFileContents( const GLchar *path );
};


#endif /* defined(__MordernOpenGl__ShaderUtils__) */
