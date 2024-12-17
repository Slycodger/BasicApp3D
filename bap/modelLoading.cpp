#include "modelLoading.h"
using namespace ModelLoading;

std::vector<std::tuple<uint, uint, uint, std::vector<Vertex>, std::vector<uint>>> meshContainers;

Mesh* processMesh(aiMesh* mesh, const aiScene* scene) {
				if (mesh->mNumVertices == 0 || mesh->mNumFaces == 0)
								return nullptr;

				Mesh* retMesh = nullptr;
				std::vector<Vertex> vertices;
				std::vector<uint> indices;
				Vertex vert;

				for (uint i = 0; i < mesh->mNumVertices; i++) {
								vert = Vertex();

								vert.position.x = mesh->mVertices[i].x;
								vert.position.y = mesh->mVertices[i].y;
								vert.position.z = mesh->mVertices[i].z;

								if (mesh->mColors[0]) {
												vert.color.x = mesh->mColors[0][i].r;
												vert.color.y = mesh->mColors[0][i].g;
												vert.color.z = mesh->mColors[0][i].b;
								}

								if (mesh->mTextureCoords[0]) {
												vert.uv.x = mesh->mTextureCoords[0][i].x;
												vert.uv.y = mesh->mTextureCoords[0][i].y;
								}

								vertices.push_back(vert);
				}

				for (uint j = 0; j < mesh->mNumFaces; j++) {
								aiFace face = mesh->mFaces[j];
								for (uint k = 0; k < face.mNumIndices; k++) {
												indices.push_back(face.mIndices[k]);
								}
				}

				retMesh = new Mesh({ vertices.data(), vertices.size() }, { indices.data(), indices.size() });

				meshContainers.push_back({ 0, 0, 0, {}, {} });

				std::get<2>(meshContainers[meshContainers.size() - 1]) = mesh->mNumFaces;
				std::get<3>(meshContainers[meshContainers.size() - 1]) = std::move(vertices);
				std::get<4>(meshContainers[meshContainers.size() - 1]) = std::move(indices);
				createMeshBuffers(
								std::get<0>(meshContainers[meshContainers.size() - 1]),
								std::get<1>(meshContainers[meshContainers.size() - 1]),
								reinterpret_cast<float*>(retMesh->vertices.data()),
								retMesh->indices.data(),
								retMesh->vertices.size_bytes(),
								retMesh->indices.size_bytes());

				retMesh->VBO = &std::get<0>(meshContainers[meshContainers.size() - 1]);
				retMesh->EBO = &std::get<1>(meshContainers[meshContainers.size() - 1]);
				retMesh->triCount = &std::get<2>(meshContainers[meshContainers.size() - 1]);

				return retMesh;

				//Object* obj = new Object;



}

void processNode(aiNode* node, const aiScene* scene) {
				for (uint i = 0; i < node->mNumMeshes; i++) {
								aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
								loadMesh(processMesh(mesh, scene), mesh->mName.C_Str());
				}

				for (uint i = 0; i < node->mNumChildren; i++) {
								processNode(node->mChildren[i], scene);
				}
}


namespace ModelLoading {
				void start(std::string str) {
								Assimp::Importer importer;
								importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
								const aiScene* scene = importer.ReadFile(str, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
								if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
												std::cout << "Failed to load model\n";
												return;
								}

								processNode(scene->mRootNode, scene);
				}
				
				void end() {
								for (auto& g : meshContainers) {
												glDeleteBuffers(1, &std::get<0>(g));
												glDeleteBuffers(1, &std::get<1>(g));
								}
				}
}