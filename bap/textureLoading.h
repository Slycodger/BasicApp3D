#pragma once
#include "stb_image.h"
#include "openGL.h"
#include <string>
#include <map>

class Texture {
public:
				unsigned int texture;
				int width;
				int height;
				
				Texture() : texture(0), width(0), height(0) {}
				Texture(unsigned int tex, int w, int h) : texture(tex), height(h), width(w) {}
};

bool loadTexture(std::string name, Texture& texture);