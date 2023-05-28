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
	
	//ʹ��/�������
	void use();
	//uniform���ߺ���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setVec3(const std::string& name, glm::vec3 value) const;
};

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	//1.���ļ��л�ȡ����/ƬԪ��ɫ��
	//-------------------------------------------
	string vertexCode;
	string fragmentCode;
	ifstream vertexShaderFile;
	ifstream fragmentShaderFile;
	//ȷ��ifstream��������׳��쳣
	vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	
	//���ļ���ȡ��ɫ������
	try {
		//���ļ�
		vertexShaderFile.open(vertexPath);
		fragmentShaderFile.open(fragmentPath);
		
		//��ȡ�ļ��Ļ������ݵ���������
		stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		
		//�ر��ļ�������
		vertexShaderFile.close();
		fragmentShaderFile.close();
		
		//ת����������string
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl; 
	}
	
	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragmentShaderCode = fragmentCode.c_str();
	
	//2.������ɫ��
	//-------------------------------------------
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	
	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	//������Ϣ
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << endl;
	}
	
	//ƬԪ��ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	//������Ϣ
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success){
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << endl;
	}
	
	//��ɫ������
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	//������Ϣ
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success){
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << endl;
	}
	
	//ɾ����ɫ��
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
