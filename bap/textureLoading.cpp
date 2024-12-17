#define STB_IMAGE_IMPLEMENTATION
#include "textureLoading.h"

std::map<std::string, Texture> globalTextures;

void includeTexture(std::string name, Texture& texture) {
				if (!texture.texture)
								return;

				globalTextures.insert({ name, texture });
}

void removeTexture(std::string name) {
				globalTextures.erase(name);
}

bool loadTexture(std::string name, Texture& texture) {
				if (globalTextures.contains(name))
								texture = globalTextures[name];


    glDeleteTextures(1, &texture.texture);
    glGenTextures(1, &texture.texture);

    glBindTexture(GL_TEXTURE_2D, texture.texture);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unsigned char* imageData = nullptr;
    int channels = 0;

    imageData = stbi_load(("textures/" + name).c_str(), &texture.width, &texture.height, &channels, 0);

    if (imageData) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(imageData);
								globalTextures.insert({ name, texture });
        return true;
    }
    else {
        stbi_image_free(imageData);
        return false;
    }
}