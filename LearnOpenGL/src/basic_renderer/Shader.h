#ifndef SHADER_H
#define SHADER_H

#include "Render.h"

#include <string>

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_Filepath;
private:
	ShaderSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int Type, const std::string& ShaderSource);
	unsigned int CreateShader(const std::string& VertexSource, const std::string& FragmentSource);
public:
	explicit Shader(const std::string& filepath);
	Shader(const Shader&) = delete;
	~Shader();
	void Bind()const;
	void Unbind()const;
	void SetBool(const std::string& uniformname, bool value)const;
	void SetInt(const std::string& uniformname, int value)const;
	void SetFloat(const std::string& uniformname, float value)const;
	void SetFloat4(const std::string& uniformname, float v0, float v1, float v2, float v3)const;
	void SetMatrix4fv(const std::string& iniformname, const float* ptr);
};

#endif