#include "flowControl.h"
#include "globalVars.h"
#include "objects.h"
#include "camera.h"
#include "input.h"
#include "globalScripts.h"
#include "progUI.h"
#include "textureLoading.h"
#include "progBoiler.h"

extern Camera _mainCamera;

Object* model = nullptr;
Object* square = nullptr;
Object* textObj = nullptr;

TextBox* textTest = nullptr;

uint textObjTexture = 0;
int width = 0;
int height = 0;

void progStart() {
				progUI::fps();

				_hideMouse = true;

    setBgColor(0.2, 0.3, 0.3, 1);
    _windowTitle = "Model Testing";

				model = createObj("Cube");
				model->transform.position = { 0, 0, -2 };
				model->transform.scale = { 0.5f };
				model->color = { 1, 1, 1, 1 };

}

void progUpdate() {
				if (keyAction::keyPressed(GLFW_KEY_ESCAPE)) {
								_closeApp = true;
				}
				if (keyAction::keyPressed(GLFW_KEY_Q)) {
								_hideMouse = !_hideMouse;
				}

				Movement::camera(25);
				Movement::freeMove(0.5f);

}

void progEnd() {

}