#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h" // include glad to get all the required OpenGL headers

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
	//ID
	unsigned int ID;

	//constructor
	Shader(const char* vertexPath, const char* fragmentPath);
	
	//使用/激活程序
	void use();
	//uniform工具函数
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
};

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	//1.从文件中获取顶点/片元着色器
	//-------------------------------------------
	string vertexCode;
	string fragmentCode;
	ifstream vertexShaderFile;
	ifstream fragmentShaderFile;
	//确保ifstream对象可以抛出异常
	vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	
	//从文件读取着色器代码
	try {
		//打开文件
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		
		//读取文件的缓冲内容到数据流中
		stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		
		//关闭文件处理器
		vertexShaderFile.close();
		fragmentShaderFile.close();
		
		//转换数据流到string
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl; 
	}
	
	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();
	
	//2.编译着色器
	//-------------------------------------------
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	
	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	//报错信息
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << endl;
	}
	
	//片元着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	//报错信息
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << endl;
	}
	
	//着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//报错信息
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << endl;
	}
	
	//删除着色器
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::setMat4(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setVec3(const std::string& name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

#endif // SHADER_H
