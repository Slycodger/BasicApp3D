#pragma once
#include "stb_image.h"
#include "openGL.h"
#include <string>

bool loadTexture(std::string name, unsigned int& textureOut, int& width, int& height);