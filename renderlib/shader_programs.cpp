#include <iostream>
#include "shaders.h"
#include "shader_programs.h"
#include "logging.h"

using namespace renderlib;

ShaderException::ShaderException(const std::string& msg): 
	msg_(msg)
{
}

const char* ShaderException::what() const throw()
{
    return msg_.c_str();
}

ShaderProgram::ShaderProgram(): 
	program_id_(0), vertex_shader_id_(0), fragment_shader_id_(0)
{
    program_id_ = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(program_id_);
    glDeleteShader(vertex_shader_id_);
    glDeleteShader(fragment_shader_id_);
}

void ShaderProgram::dump_compile_log_for(GLuint id)
{
    char* log = new char[10000];
    int len;
    glGetShaderInfoLog(id, 10000, &len, log);
	LOGW("Dumping error log");
	LOGW(log);
    delete[] log;
}


void ShaderProgram::compile_shader_pair(const char* vertex_shader_src, const char* fragment_shader_src)
{    
    vertex_shader_id_ = compile(vertex_shader_src, GL_VERTEX_SHADER);
    fragment_shader_id_ = compile(fragment_shader_src, GL_FRAGMENT_SHADER);
    
    glAttachShader(program_id_, vertex_shader_id_);
    glAttachShader(program_id_, fragment_shader_id_);
    glLinkProgram(program_id_);
    GLint status;
    glGetProgramiv(program_id_, GL_LINK_STATUS, &status);
    if (status == 0) {
        throw ShaderException("Could not link shader program");
    }
}

GLuint ShaderProgram::compile(const char* shader_src, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, &shader_src, NULL);
    glCompileShader(shader);
    
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == 0) {
        dump_compile_log_for(shader);
        glDeleteShader(shader);
        if (shader_type == GL_VERTEX_SHADER) {
            throw ShaderException("Could not compile vertex shader");
        } else if (shader_type == GL_FRAGMENT_SHADER) {
            throw ShaderException("Could not compile fragment shader");
        } else {
            throw ShaderException("Could not compile shader");
        }
    }
    
    return shader;
}

GLint ShaderProgram::get_uniform(const char* uniform_name)
{
    GLint result = glGetUniformLocation(program_id(), uniform_name);
    if (result == -1) {
        throw ShaderException("Could not find uniform");
    }
    return result;
}

GLint ShaderProgram::get_attribute(const char* attribute_name)
{
    GLint result = glGetAttribLocation(program_id(), attribute_name);
    if (result == -1) {
        throw ShaderException("Could not find attribute");
    }
    glEnableVertexAttribArray(result);
    return result;
}

SimpleShaderProgram::SimpleShaderProgram()
{
	compile_shader_pair(SIMPLE_VERTEX_SHADER, SIMPLE_FRAGMENT_SHADER);
	uMVPMatrix = get_uniform("uMVPMatrix");
	aPosition = get_attribute("aPosition");
	aColor = get_attribute("aColor");
}

RegularShaderProgram::RegularShaderProgram()
{
	compile_shader_pair(REGULAR_VERTEX_SHADER, REGULAR_FRAGMENT_SHADER);
	uMVPMatrix = get_uniform("uMVPMatrix");
	aPosition = get_attribute("aPosition");
	aNormal = get_attribute("aNormal");
	aColor = get_attribute("aColor");
	uAmbientColor = get_uniform("uAmbientColor");
	uLight0Dir = get_uniform("uLight0Dir");
	uLight0Clr = get_uniform("uLight0Clr");
	uLight1Dir = get_uniform("uLight1Dir");
	uLight1Clr = get_uniform("uLight1Clr");
	uLight2Dir = get_uniform("uLight2Dir");
	uLight2Clr = get_uniform("uLight2Clr");
}
