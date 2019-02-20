#include "Shader.h"
#include <Windows.h>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	std::string vertexCode;
	std::string fragmenCode;

	std::ifstream vertexFile;
	std::ifstream fragmentFile;

	vertexFile.exceptions(std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::badbit);

	try
	{
		std::locale loc = std::locale::global(std::locale(""));//设置全局locale为本地环境

		std::stringstream vertexStream, fragmentStream;

		vertexFile.open(vertexPath);
		if (vertexFile.is_open())
		{
			vertexStream << vertexFile.rdbuf();
			vertexCode = vertexStream.str();
		}
		else
		{
			std::cout << "Open file error:"<< GetLastError() << std::endl;
		}
		fragmentFile.open(fragmentPath);
		if (fragmentFile.is_open())
		{
			fragmentStream << fragmentFile.rdbuf();
			fragmenCode = fragmentStream.str();
		}
		else
		{
			std::cout << "Open file error:"<< GetLastError() << std::endl;
		}
		vertexFile.close();
		fragmentFile.close();


		std::locale::global(loc);//恢复全局locale
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
	const GLchar* vCode = vertexCode.c_str();
	const GLchar* fCode = fragmenCode.c_str();
	
	GLuint vertex, fragment;

	GLchar infoLog[512];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &mStatu);
	if (!mStatu)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<std::endl;
	}
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, nullptr);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &mStatu);
	if (!mStatu)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);
	glLinkProgram(mProgram);

	glGetProgramiv(mProgram, GL_LINK_STATUS, &mStatu);
	if (!mStatu)
	{
		glGetProgramInfoLog(mProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);


}

Shader::~Shader()
{
}

void Shader::Use()
{
	glUseProgram(mProgram);
}
