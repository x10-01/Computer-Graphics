#pragma once
#include <fstream>
#include <sstream>
#include <string>

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		shaderProgram = glCreateProgram();
		CompileShaderFromFile(vertexPath, GL_VERTEX_SHADER);
		CompileShaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);
		LinkShaderProgram();
	}
	~Shader()
	{
		glDeleteProgram(shaderProgram);
	}
	void UseProgram()
	{
		glUseProgram(shaderProgram);
	}

private:
	GLuint shaderProgram;
	void CompileShaderFromFile(const char* path, GLenum type)
	{
		std::string code;
		std::ifstream shaderFile;
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			shaderFile.open(path);
			std::stringstream shaderStream;
			shaderStream << shaderFile.rdbuf();
			shaderFile.close();
			code = shaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::FILE_NOT_READ" << e.what() << std::endl;
		}
		const GLchar* shaderCode = code.c_str();
		GLuint shader = glCreateShader(type);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::COMPILATION_FAIL\n" << infoLog << std::endl;
			exit(-1);
		}
		glAttachShader(shaderProgram, shader);
		glDeleteShader(shader);
	}
	void LinkShaderProgram()
	{
		glLinkProgram(shaderProgram);
		GLint success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINK_FAIL\n" << infoLog << std::endl;
			exit(-1);
		}
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::LINK_FAIL\n" << infoLog << std::endl;
			exit(-1);
		}
#ifdef __WIN64
		glValidateProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
		if (!success)
		{
			GLchar infoLog[1024];
			glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::VALIDATE_FAIL\n" << infoLog << std::endl;
			exit(-1);
		}
#endif 
	};
};