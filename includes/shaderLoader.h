#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>

enum ShaderType {
    VERTEX,
    FRAGMENT
};

enum ShaderCreationMode {
	BOTH_FROM_FILE,
	BOTH_FROM_ID,
	VERTEX_ID_FRAG_FILE,
	VERTEX_FILE_FRAG_ID
};

class shaderLoader
{
public:
	shaderLoader(std::string vertexShaderPath, std::string fragmentShaderPath);
	shaderLoader(unsigned int vertexShaderID, unsigned int fragmentShaderID);
	shaderLoader(std::string vertexShaderPath, unsigned int fragmentShaderID);
	shaderLoader(unsigned int vertexShaderID, std::string fragmentShaderPath);
	~shaderLoader() {
		glDeleteProgram(shaderProgram);
		// You can also delete the shaders themselves after linking
	}

	std::string getVertexShaderSource() {
		return vertexShaderSource;
	}

	std::string getFragmentShaderSource() {
		return fragmentShaderSource;
	}

	unsigned int getVertexShaderID() {
		return vertexShader;
	}

	unsigned int getFragmentShaderID() {
		return fragmentShader;
	}

	unsigned int getProgramID() {
		return shaderProgram;
	}

	int getUniformLocation(const char* varName) {
		return glGetUniformLocation(shaderProgram, varName);
	}

	void createShaders(ShaderCreationMode mode);
	void useProgram();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

    
private:
	void getShaderCompilationError(unsigned int shaderID, GLenum ERROR_TYPE, ShaderType type);
    void getShaderLinkError(unsigned int program, GLenum ERROR_TYPE);

	void compileVertexShader();
	void compileFragmentShader();
	void linkShaders();

    unsigned int vertexShader;
    unsigned int fragmentShader;
	unsigned int shaderProgram;

	void loadShaders(std::string vPath, std::string fPath);
	void loadVertexShader(std::string vPath);
	void loadFragmentShader(std::string fPath);


	std::string vertexShaderSource;
	std::string fragmentShaderSource;
};
