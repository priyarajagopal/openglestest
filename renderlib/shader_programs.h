#pragma once

#include <exception>
#include <string>
#include "gles.h"

namespace renderlib {

class ShaderException : public std::exception {
public:
    explicit ShaderException(const std::string&);
    ~ShaderException() throw() { }
    
    virtual const char* what() const throw();
    
private:
    const std::string msg_;
};

class ShaderProgram {
public:
    virtual ~ShaderProgram();
    
    GLuint program_id() const { return program_id_; }
    
protected:
    ShaderProgram();
    
    void compile_shader_pair(const char* vertex_shader_src, const char* fragment_shader_src);
    
    GLint get_uniform(const char* uniform_name);
    GLint get_attribute(const char* attribute_name);

    void dump_compile_log_for(GLuint id);
    
private:
    ShaderProgram(const ShaderProgram&);
    ShaderProgram& operator=(const ShaderProgram&);

	GLuint compile(const char* shader_src, GLenum shader_type);
    
    GLuint program_id_;
    GLuint vertex_shader_id_;
    GLuint fragment_shader_id_;
};

class SimpleShaderProgram : public ShaderProgram {
public:
    SimpleShaderProgram();

    GLint uMVPMatrix, aPosition, aColor;
};

class RegularShaderProgram : public ShaderProgram {
public:
    RegularShaderProgram();

    GLint uMVPMatrix, 
		aPosition, aNormal, aColor,
		uAmbientColor, 
		uLight0Dir, uLight0Clr, uLight1Dir, uLight1Clr, uLight2Dir, uLight2Clr;
};

}
