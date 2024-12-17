#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "textureLoading.h"
#include "objects.h"

namespace ModelLoading {
				void start(std::string);
				void end();
}