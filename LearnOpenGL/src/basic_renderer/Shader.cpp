#include "shader.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

ShaderSource Shader::ParseShader(const std::string& filepath)
{
	std::ifstream ifs(filepath, std::ios::in);
	if (ifs.is_open())
	{
		std::string line;
		std::stringstream ss[2];
		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};
		ShaderType Type = ShaderType::NONE;
		while (getline(ifs, line))
		{
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					Type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					Type = ShaderType::FRAGMENT;
			}
			else
			{
				ss[(int)Type] << line << '\n';
			}
		}
		return { ss[0].str(),ss[1].str() };
	}
	else
	{
		std::cout << "Openning file failed!\n";
		return { "\0","\0" };
	}
}

unsigned int Shader::CompileShader(unsigned int Type, const std::string& ShaderSource)
{
	unsigned int shader = glCreateShader(Type);
	const char* source = ShaderSource.c_str();
	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);
	//Error Handling
	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		(glGetShaderInfoLog(shader, length, &length, message));
		std::cout << "Failed to compile "
			<< (Type == GL_VERTEX_SHADER ? "vertex " : "fragment ")
			<< "shader" << std::endl;
		std::cout << message << std::endl;
		(glDeleteShader(shader));
		return 0;
	}
	return shader;
}

unsigned int Shader::CreateShader(const std::string& VertexSource, const std::string& FragmentSource)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, VertexSource);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, FragmentSource);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	glValidateProgram(program);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

Shader::Shader(const std::string& filepath)
	:m_RendererID(0), m_Filepath(filepath)
{
	ShaderSource shadersource = ParseShader(filepath);
	m_RendererID = CreateShader(shadersource.VertexSource, shadersource.FragmentSource);
}

Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}

void Shader::Bind()const
{
	glUseProgram(m_RendererID);
}

void Shader::Unbind()const
{
	glUseProgram(0);
}

void Shader::SetBool(const std::string& uniformname, bool value)const
{
	glUniform1i(glGetUniformLocation(m_RendererID, uniformname.c_str()), value);
}

void Shader::SetInt(const std::string& uniformname, int value)const
{
	glUniform1i(glGetUniformLocation(m_RendererID, uniformname.c_str()), value);
}

void Shader::SetFloat(const std::string& uniformname, float value)const
{
	glUniform1f(glGetUniformLocation(m_RendererID, uniformname.c_str()), value);
}

void Shader::SetFloat4(const std::string& uniformname, float v0, float v1, float v2, float v3)const
{
	glUniform4f(glGetUniformLocation(m_RendererID, uniformname.c_str()), v0, v1, v2, v3);
}

void Shader::SetMatrix4fv(const std::string& uniformname,const float* ptr)
{
	glUniformMatrix4fv(glGetUniformLocation(m_RendererID, uniformname.c_str()), 1, GL_FALSE, ptr);
}