#pragma once

#include <string>
#include <glad/glad.h>
class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture() {
		free(data);
	}
	unsigned int getWidth() {
		return width;
	}

	unsigned int getHeight() {
		return height;
	}

	unsigned int getNrChannels() {
		return nrChannels;
	}

	// Should only be used this to read data and not modify
	const unsigned char* getData() {
		return data;
	}
	static unsigned int loadTexture(char const* path);
private:
	unsigned int width;
	unsigned int height;
	unsigned int nrChannels;
	unsigned char* data;
};

