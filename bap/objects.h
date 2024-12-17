#pragma once
#if defined(_OBJECTS) || defined(_FLOW_CONTROL)
#include <map>
#include <stdexcept>
#include "constants.h"
#include <algorithm>
#include "intializeShader.h"
#include "openGL.h"
#include "GlobalVars.h"
#include "camera.h"
namespace Objects {
	void start();
	void end();
}
#endif

#include "objScripts.h"
#include "typeDefs.h"
#include "vector.h"
#include <Set>
#include <string>
#include <Vector>
#include <limits>
#include "shapes.h"
#include "mathConstants.h"
#include <span>

class Vertex {
public:
				Vec3 position;
				Vec3 color;
				Vec2 uv;

				Vertex() : position(0), color(1), uv(0) {}
				Vertex(Vec3 Pos, Vec3 C, Vec2 Uv) : position(Pos), color(C), uv(Uv) {}
};

class Mesh {
public:
				std::span<Vertex> vertices;
				std::span<uint> indices;
				uint* VBO = nullptr;
				uint* EBO = nullptr;
				uint* triCount = nullptr;


				Mesh() : vertices(), indices(), VBO(nullptr), EBO(nullptr), triCount(nullptr) {}
				Mesh(std::span<Vertex> verts, std::span<uint> tris) : vertices(verts), indices(tris), VBO(nullptr), EBO(nullptr), triCount(nullptr) {}
				Mesh(std::span<Vertex> verts, std::span<uint> tris, uint* vbo, uint* ebo, uint* tCount) : vertices(verts), indices(tris), VBO(vbo), EBO(ebo), triCount(tCount) {}
};

class Transform {
public:
				Vec3 position = 0;
				Vec3 scale = 1;
				Vec3 rotation = 0;

				Transform() : position(0), scale(1), rotation(0) {}
};

class Object {
public:
				Transform transform;
				Transform relativeTransform;
				std::set<Object*> children;
				std::set<Object*> dependencies;
				Object* dependent = nullptr;
				Object* parent = nullptr;
				Vec4 color = 1;
				bool active = true;
				std::string objType;
				bool scriptCreated = false;
				bool ui = false;
				bool weak = false;
				std::vector<void*> scripts;
				Mesh* mesh = nullptr;
				uint index = -1;
				bool directInstance = false;
				uint* texture = nullptr;



				Object& operator=(Object* obj) {
								this->transform = obj->transform;

								void addObjScript(Object * obj, void* script, char c);
								for (auto& scrV : obj->scripts) {
												void* newScr = ((scriptBase*)scrV)->getNew();
												addObjScript(this, newScr, 'c');
								}

								for (auto child : obj->children) {
												if (child->scriptCreated)
																continue;
												Object* temp = new Object;
												*temp = child;
												temp->setParent(this);
								}

								this->color = obj->color;
								this->active = obj->active;
								this->objType = obj->objType;
								this->active = obj->active;
								this->texture = obj->texture;

								if (!obj->mesh)
												return *this;

								this->mesh = obj->mesh;

								return *this;
				}


				//-------------------Constructor
				Object() : transform(), relativeTransform(), children(), dependencies(), parent(nullptr), color(1)
								, active(true), scriptCreated(false), ui(false), scripts(), mesh(nullptr), index(-1),
				directInstance(false), texture(nullptr)
				{}

				//-------------------Destructor
				~Object() {}

				//--------------------------getter functions

				template <typename T>
				T* getScriptType() {
								for (auto scrV : scripts) {
												scriptBase* scr = (scriptBase*)scrV;
												if (scr->getName() == T::name)
																return (T*)scrV;
								}
				}


				//-------------------------Parenting functions

				void setParent(Object* obj) {
								if (parent != nullptr)
												parent->children.erase(this);

								obj->children.insert(this);
								parent = obj;
								this->ui = parent->ui;
								relativeTransform.position = (transform.position - parent->transform.position) / parent->transform.scale;
								relativeTransform.scale = transform.scale / parent->transform.scale;
								relativeTransform.rotation = transform.rotation - parent->transform.rotation;
				}

				void setToRelative() {
								if (parent == nullptr)
												return;

								this->ui = parent->ui;

								glm::mat4 Translation(1);
								Translation = glm::rotate(Translation, glm::radians<float>(parent->transform.rotation.x), glm::vec3(1, 0, 0));
								Translation = glm::rotate(Translation, glm::radians<float>(parent->transform.rotation.y), glm::vec3(0, 1, 0));
								Translation = glm::rotate(Translation, glm::radians<float>(parent->transform.rotation.z), glm::vec3(0, 0, 1));
								Translation = glm::translate(Translation, (relativeTransform.position * parent->transform.scale + parent->transform.position).toGLM());

								transform.position = Translation * glm::vec4(0, 0, 0, 1);
								transform.rotation = parent->transform.rotation;
								transform.scale = parent->transform.scale * relativeTransform.scale;
				}
				void removeParent() {
								if (parent == nullptr)
												return;
								parent->children.erase(this);

								parent = nullptr;
				}


				//Dependent object stuff

				void setDependent(Object* obj) {
								removeDependent();

								dependent = obj;
								obj->dependencies.insert(this);
				}

				void removeDependent() {
								if (dependent == nullptr)
												return;
								dependent->dependencies.erase(this);
								dependent = nullptr;
				}



				//------------------------------Checking functions

				static bool chainActive(Object*& obj) {
								if (!obj->active)
												return false;
								if (obj->parent == nullptr)
												return true;
								if (!obj->parent->active)
												return false;
								return chainActive(obj->parent);
				}
};

Object* createObj(std::string objName);
bool unloadMesh(std::string name);
bool loadMesh(Mesh* mesh, std::string name);
void deleteObjMeshes();
void createMeshBuffers(uint& VBO, uint& EBO, float* vertices, uint* indices, size_t vertSize, size_t indiceSize);
void createMeshBuffers(uint& VBO, uint& EBO, const float* vertices, const uint* indices, size_t vertSize, size_t indiceSize);
void addObjScript(Object* obj, void* script);
void removeObjScript(Object* obj, unsigned int index);
unsigned int getObjScriptIndex(Object*& obj, std::string name);
void updateObjScripts(Object*& obj);
void clearObjScripts(Object* const& obj);
void updateObjChildren(Object*& obj);
void clearObjChildren(Object*& obj);
void deleteObj(Object*& obj);
void deleteObj(Object*& obj, int);
void deleteAll();
void drawObjStencil(Object* obj);
void drawAllObjs();
uint findObjSlot();
bool addGlobalObj(Object*& obj);
Object* createObj();
Object* instantiateObj(std::string objName);
bool saveObj(Object* obj, std::string objName);
void addObjScript(Object* obj, void* script, char c);
void deleteObj(Object*& obj, const char);
void startObj(Object* obj);
void createScriptObjs(std::pair<std::vector<std::string>, std::vector<void*>&> pair);
glm::mat4 createObjTransform(Object* obj);
bool setObjMesh(Object* obj, std::string mesh);

extern Object* globalObjects[];
extern uint objCount;