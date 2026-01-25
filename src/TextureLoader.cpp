#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const std::string& filePath) {
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(filePath.c_str(), (int*)&width, (int*)&height, (int*)&nrChannels, 0);
	if (data == NULL) {
		printf("loading failed\n");
	}
};