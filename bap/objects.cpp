#define _OBJECTS
#include "objects.h"

static std::map<std::string, Mesh*> objMeshes;
static std::map<std::string, Object*> savedObjs;

unsigned int squareVBO, squareEBO, squareTriCount;
unsigned int triangleVBO, triangleEBO, triangleTriCount;
unsigned int cubeVBO, cubeEBO, cubeTriCount;

uint objCount = 0;
Object* globalObjects[_MAX_OBJECTS];

static glm::mat4 _transform(1);

Camera _mainCamera = Camera(0, 0, 70, 0.01f, 100.f);

static Shader shader;
static uint VAO;

const float* _identityMatrix = nullptr;

bool _showUI = true;

namespace Objects {
				//Creates default objects
				void start() {
								createMeshBuffers(squareVBO, squareEBO, squareVertices, squareIndices, sizeof(squareVertices), sizeof(squareIndices));
								squareTriCount = 2;
								loadMesh("square", new Mesh(&squareVBO, &squareEBO, &squareTriCount));

								createMeshBuffers(triangleVBO, triangleEBO, triangleVertices, triangleIndices, sizeof(triangleVertices), sizeof(triangleIndices));
								triangleTriCount = 1;
								loadMesh("triangle", new Mesh(&triangleVBO, &triangleEBO, &triangleTriCount));

								createMeshBuffers(cubeVBO, cubeEBO, cubeVertices, cubeIndices, sizeof(cubeVertices), sizeof(cubeIndices));
								cubeTriCount = 12;
								loadMesh("cube", new Mesh(&cubeVBO, &cubeEBO, &cubeTriCount));


								_mainCamera.setPerspectiveView(70, _screenRatio, 0.01f, 100.f);
								_mainCamera.setOrthographicView(_screenRatio, 1, 1);

								_identityMatrix = new float [16] { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
				}

				//Deletes at the end
				void end() {
								unloadMesh("square");
								unloadMesh("triangle");
								unloadMesh("cube");
								glDeleteBuffers(1, &squareVBO);
								glDeleteBuffers(1, &squareEBO);
								glDeleteBuffers(1, &triangleVBO);
								glDeleteBuffers(1, &triangleEBO);
								deleteObjMeshes();

								for (auto savedObj : savedObjs) {
												deleteObj(savedObj.second, 'c');
								}
								savedObjs.clear();
								delete[](_identityMatrix);
				}
}


//---------------------OBJECT----HANDLING


bool saveObj(Object* obj, std::string objName) {
				if (savedObjs.count(objName))
								return false;

				Object* objCopy = new Object;
				*objCopy = obj;

				return savedObjs.insert(std::pair<std::string, Object*>{objName, objCopy}).second;
}

Object* instantiateObj(std::string objName) {
				if (!savedObjs.count(objName))
								return nullptr;

				Object* ret = createObj();
				*ret = savedObjs.at(objName);
				for (Object* child : ret->children) {
								addGlobalObj(child);
				}
				startObject(ret);

				return ret;
}

//Creates the buffers for an object
void createMeshBuffers(uint& VBO, uint& EBO, const float* vertices, const uint* indices, const size_t vertSize, const size_t indiceSize) {
				glCreateBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vertSize, vertices, GL_STATIC_DRAW);

				glCreateBuffers(1, &EBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceSize, indices, GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Loads an object into a map
bool loadMesh(std::string name, Mesh* mesh) {
				return objMeshes.insert({ name, mesh }).second;
}

//Removes a specific object type from the map
bool unloadMesh(std::string name) {
				if (!objMeshes.contains(name))
								return false;

				delete (objMeshes[name]);
				objMeshes.erase(name);
}

//Deletes all object types
void deleteObjMeshes() {
				for (auto& mesh : objMeshes) {
								delete (mesh.second);
				}

				objMeshes.clear();
}

//Create an object with a mesh
Object* createObj(std::string mesh) {
				if (!objMeshes.count(mesh))
								return nullptr;
				auto& objData = objMeshes.at(mesh);

				Object* obj = new Object();

				obj->mesh = objData;
				obj->objType = mesh;

				if (!addGlobalObj(obj))
				{
								delete(obj);
								return nullptr;
				}

				return obj;
}

//Creates an object with no mesh
Object* createObj() {
				Object* obj = new Object();

				if (!addGlobalObj(obj)) {
								delete(obj);
								return nullptr;
				}

				return obj;
}

bool setObjMesh(Object* obj, std::string mesh) {
				if (!objMeshes.contains(mesh) || !obj)
								return false;

				obj->mesh = objMeshes[mesh];

				return true;
}

//Finds the first open slot in 'globalObjects'
uint findObjSlot() {
				for (uint i = 0; i < _MAX_OBJECTS; i++) {
								if (globalObjects[i] == nullptr)
												return i;
				}
				return _MAX_OBJECTS;
}

//Returns if it is able to add an object to 'globalObjects'
bool addGlobalObj(Object*& obj) {
				if (obj->index > -1)
								return false;
				uint objSlot = findObjSlot();
				if (objSlot == _MAX_OBJECTS)
								return false;

				globalObjects[objSlot] = obj;
				globalObjects[objSlot]->index = objSlot;

				objCount++;
				return true;
}

//Deletes all objects
void deleteAll() {
				for (uint i = 0; i < _MAX_OBJECTS; i++) {
								if (globalObjects[i] == nullptr || ((Object*)globalObjects[i])->parent != nullptr)
												continue;
								clearObjScripts((Object*&)globalObjects[i]);
								clearObjChildren((Object*&)globalObjects[i]);
								delete(globalObjects[i]);
								globalObjects[i] = nullptr;
				}
				objCount = 0;
}

//Deletes an object without removing from global objects, meant for objects that are self-contained elsewhere
void deleteObj(Object*& obj, const char) {
				if (obj == nullptr)
								return;
				clearObjScripts(obj);
				clearObjChildren(obj);
				obj->removeParent();
				obj->removeDependent();

				delete(obj);
				obj = nullptr;
}

//Deletes an object
void deleteObj(Object*& obj) {
				if (obj == nullptr)
								return;
				int index = obj->index;
				clearObjScripts(obj);
				clearObjChildren(obj);
				obj->removeParent();
				obj->removeDependent();

				delete(obj);
				if (index > -1) {
								globalObjects[index] = nullptr;
								objCount--;
				}
				obj = nullptr;
}

//Deletes an object without updating the parent
void deleteObj(Object*& obj, int) {
				if (obj == nullptr)
								return;
				uint index = obj->index;
				clearObjScripts(obj);
				clearObjChildren(obj);
				obj->removeDependent();

				delete(obj);
				if (index > -1) {
								globalObjects[index] = nullptr;
								objCount--;
				}
				obj = nullptr;
}


//--------------------FOR-OBJECTS

//Starts an objects scripts
void startObject(Object* obj) {
				for (auto scr : obj->scripts) {
								if (!((scriptBase*)scr)->started) {
												createScriptObjs(((scriptBase*)scr)->objsNeeded());
												((scriptBase*)scr)->start();
								}
				}
				for (auto child : obj->children) {
								startObject(child);
				}
}

//Creates the objects needed for a script
void createScriptObjs(std::pair<std::vector<std::string>, std::vector<void*>&> pair) {
				for (auto str : pair.first) {
								pair.second.push_back(createObj(str));
								((Object*)pair.second[pair.second.size() - 1])->scriptCreated = true;
				}
}

//Adds an object script
void addObjScript(Object* obj, void* script) {
				obj->scripts.push_back(script);
				scriptBase* realScr = (scriptBase*)script;
				createScriptObjs(realScr->objsNeeded());
				realScr->vThisObj = obj;
				realScr->start();
}

//Add an object script without starting it
void addObjScript(Object* obj, void* script, char c) {
				obj->scripts.push_back(script);
				scriptBase* realScr = (scriptBase*)script;
				realScr->vThisObj = obj;
}

//Remove an object script at a specific index
void removeObjScript(Object* obj, unsigned int index) {
				((scriptBase*)(obj->scripts[index]))->end();
				delete(obj->scripts[index]);
				obj->scripts.erase(obj->scripts.begin() + index);
}

//Get the index of a script on an object with a certain name
unsigned int getObjScriptIndex(Object*& obj, std::string name) {
				for (uint i = 0; i < obj->scripts.size(); i++) {
								scriptBase* realScr = (scriptBase*)obj->scripts[i];
								if (realScr->getName() == name) {
												return i;
								}
				}
				return UINT_MAX;
}


//Updates an objects scripts
void updateObjScripts(Object*& obj) {
				for (uint i = 0; i < obj->scripts.size(); i++) {
								scriptBase* realScr = (scriptBase*)obj->scripts[i];
								realScr->update();
				}
}

//Deletes an objects scripts
void clearObjScripts(Object* const& obj) {
				for (uint i = 0; i < obj->scripts.size(); i++) {
								((scriptBase*)obj->scripts[i])->end();
								delete(obj->scripts[i]);
				}
				obj->scripts.clear();
}

//Deletes an objects children
void clearObjChildren(Object*& obj) {
				if (obj->children.size() == 0)
								return;
				for (auto child : obj->children) {
								deleteObj(child, 0);
				}
				obj->children.clear();
}

//Updates an objects children
void updateObjChildren(Object*& obj) {
				if (obj->children.size() == 0)
								return;
				for (Object* child : obj->children) {
								child->setToRelative();
								updateObjChildren(child);
				}
}

//Creates the transformation matrix for an object
glm::mat4 createObjTransform(Object* obj) {

				glm::mat4 ret = glm::mat4(1);
				ret = glm::translate(ret, obj->transform.position.toGLM());
				ret = glm::rotate(ret, (float)_degToRad * obj->transform.rotation.x, glm::vec3(1, 0, 0));
				ret = glm::rotate(ret, (float)_degToRad * obj->transform.rotation.y, glm::vec3(0, 1, 0));
				ret = glm::rotate(ret, (float)_degToRad * obj->transform.rotation.z, glm::vec3(0, 0, 1));
				ret = glm::scale(ret, obj->transform.scale.toGLM());

				return ret;
}



//---------------------OBJECT-DRAWING

//Draws all objects
void drawAllObjs() {
				if (objCount == 0)
								return;
				_mainCamera.setViewMatrix();


				uint objDrawn = 0;
				Object* prevDependent = nullptr;

				//3D objects
				for (int i = 0; i < _MAX_OBJECTS; i++) {
								if (objDrawn >= objCount)
												break;
								if (globalObjects[i] == nullptr)
												continue;

								Object*& obj = globalObjects[i];
								obj->index = i;

								if (obj->UI || !obj->mesh)
												continue;

								if (!Object::chainActive(obj)) {
												objDrawn++;
												continue;
								}

								if (obj->parent == nullptr)
												updateObjChildren(obj);
								updateObjScripts(obj);

								if (obj->dependent != nullptr) {
												glEnable(GL_STENCIL_TEST);
												if (obj->dependent != prevDependent) {
																glClear(GL_STENCIL_BUFFER_BIT);
																drawObjStencil(obj->dependent);
												}
												prevDependent = obj->dependent;
												glStencilFunc(GL_EQUAL, 0x01, 0xFF);
												glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
								}
								else
												glDisable(GL_STENCIL_TEST);

								if (obj->weak) {
												glBlendEquation(GL_FUNC_ADD);
												glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA, GL_ZERO, GL_ONE);
								}
								else {
												glBlendEquation(GL_FUNC_ADD);
												glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
								}

								_transform = createObjTransform(obj);
								if (obj->texture) {
												uint texTarget = *obj->texture;
												shader.use(getShader("textureShader"));
												if (!obj->UI) {
																shader.setMat4("view", _mainCamera.viewMatrix);
																shader.setMat4("projection", _mainCamera.perspectiveView);
												}
												else {
																shader.setMat4("projection", _mainCamera.orthographicView);
																shader.setMat4("view", _identityMatrix);
												}

												shader.setMat4("transform", _transform);
												shader.setVec4("color", obj->color);
												shader.setInt("texTarget", 0);
												glActiveTexture(GL_TEXTURE0);
												glBindTexture(GL_TEXTURE_2D, texTarget);
								}
								else {
												shader.use(getShader("noTextureShader"));
												if (!obj->UI) {
																shader.setMat4("view", _mainCamera.viewMatrix);
																shader.setMat4("projection", _mainCamera.perspectiveView);
												}
												else {
																shader.setMat4("projection", _mainCamera.orthographicView);
																shader.setMat4("view", _identityMatrix);
												}

												shader.setMat4("transform", _transform);
												shader.setVec4("color", obj->color);
								}

								glBindVertexArray(VAO);
								glBindBuffer(GL_ARRAY_BUFFER, *obj->mesh->VBO);
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *obj->mesh->EBO);

								glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
								glEnableVertexAttribArray(0);
								glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
								glEnableVertexAttribArray(1);

								glDrawElements(GL_TRIANGLES, *obj->mesh->triCount * 3, GL_UNSIGNED_INT, 0);
								objDrawn++;
				}
				glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				//UI Objects
				for (int i = 0; i < _MAX_OBJECTS; i++) {
								if (!_showUI)
												break;

								if (objDrawn >= objCount)
												break;
								if (globalObjects[i] == nullptr)
												continue;

								Object*& obj = globalObjects[i];
								obj->index = i;

								if (!obj->UI || !obj->mesh)
												continue;

								if (!Object::chainActive(obj)) {
												objDrawn++;
												continue;
								}

								if (obj->parent == nullptr)
												updateObjChildren(obj);
								updateObjScripts(obj);

								if (obj->dependent != nullptr) {
												glEnable(GL_STENCIL_TEST);
												if (obj->dependent != prevDependent) {
																glClear(GL_STENCIL_BUFFER_BIT);
																drawObjStencil(obj->dependent);
												}
												prevDependent = obj->dependent;
												glStencilFunc(GL_EQUAL, 0x01, 0xFF);
												glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
								}
								else
												glDisable(GL_STENCIL_TEST);

								if (obj->weak) {
												glBlendEquation(GL_FUNC_ADD);
												glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA, GL_ZERO, GL_ONE);
								}
								else {
												glBlendEquation(GL_FUNC_ADD);
												glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
								}

								_transform = createObjTransform(obj);
								if (obj->texture) {
												uint texTarget = *obj->texture;
												shader.use(getShader("textureShader"));
												if (!obj->UI) {
																shader.setMat4("view", _mainCamera.viewMatrix);
																shader.setMat4("projection", _mainCamera.perspectiveView);
												}
												else {
																shader.setMat4("projection", _mainCamera.orthographicView);
																shader.setMat4("view", _identityMatrix);
												}

												shader.setMat4("transform", _transform);
												shader.setVec4("color", obj->color);
												shader.setInt("texTarget", 0);
												glActiveTexture(GL_TEXTURE0);
												glBindTexture(GL_TEXTURE_2D, texTarget);
								}
								else {
												shader.use(getShader("noTextureShader"));
												if (!obj->UI) {
																shader.setMat4("view", _mainCamera.viewMatrix);
																shader.setMat4("projection", _mainCamera.perspectiveView);
												}
												else {
																shader.setMat4("projection", _mainCamera.orthographicView);
																shader.setMat4("view", _identityMatrix);
												}

												shader.setMat4("transform", _transform);
												shader.setVec4("color", obj->color);
								}

								glBindVertexArray(VAO);
								glBindBuffer(GL_ARRAY_BUFFER, *obj->mesh->VBO);
								glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *obj->mesh->EBO);

								glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
								glEnableVertexAttribArray(0);
								glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
								glEnableVertexAttribArray(1);

								glDrawElements(GL_TRIANGLES, *obj->mesh->triCount * 3, GL_UNSIGNED_INT, 0);
								objDrawn++;
				}


				glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				glBindVertexArray(0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawObjStencil(Object* obj) {
				glStencilFunc(GL_ALWAYS, 0x01, 0xFF);
				glStencilOp(GL_INCR, GL_INCR, GL_INCR);
				glDisable(GL_DEPTH_TEST);
				glColorMask(0, 0, 0, 0);

				_transform = createObjTransform(obj);

				shader.use(getShader("noTextureShader"));
				shader.setMat4("transform", _transform);
				if (!obj->UI) {
								shader.setMat4("view", _mainCamera.viewMatrix);
								shader.setMat4("projection", _mainCamera.perspectiveView);
				}
				else {
								shader.setMat4("projection", _mainCamera.orthographicView);
								shader.setMat4("view", _identityMatrix);
				}

				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, *obj->mesh->VBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *obj->mesh->EBO);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				glDrawElements(GL_TRIANGLES, *obj->mesh->triCount * 3, GL_UNSIGNED_INT, 0);

				glEnable(GL_DEPTH_TEST);
				glColorMask(1, 1, 1, 1);
}