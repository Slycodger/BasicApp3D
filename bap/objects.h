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

struct Mesh {
				uint* VBO = nullptr;
				uint* EBO = nullptr;
				uint* triCount = nullptr;
				Mesh() : VBO(nullptr), EBO(nullptr), triCount(nullptr) {}
				Mesh(uint* vbo, uint* ebo, uint* tCount) : VBO(vbo), EBO(ebo), triCount(tCount) {}
};

struct Transform {
				Vec3 position = 0;
				Vec3 scale = 1;
				Vec3 rotation = 0;

				Transform() : position(0), scale(1), rotation(0) {}
};

struct Object {
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
				bool UI = false;
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
								, active(true), scriptCreated(false), UI(false), scripts(), mesh(nullptr), index(-1),
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
								this->UI = parent->UI;
								relativeTransform.position = (transform.position - parent->transform.position) / parent->transform.scale;
								relativeTransform.scale = transform.scale / parent->transform.scale;
								relativeTransform.rotation = transform.rotation - parent->transform.rotation;
				}

				void setToRelative() {
								if (parent == nullptr)
												return;

								this->UI = parent->UI;

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
bool loadMesh(std::string name, Mesh* mesh);
void deleteObjMeshes();
void createMeshBuffers(uint& VBO, uint& EBO, const float* vertices, const uint* indices, const size_t vertSize, const size_t indiceSize);
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
void startObject(Object* obj);
void createScriptObjs(std::pair<std::vector<std::string>, std::vector<void*>&> pair);
glm::mat4 createObjTransform(Object* obj);
bool setObjMesh(Object* obj, std::string mesh);

extern Object* globalObjects[];
extern uint objCount;