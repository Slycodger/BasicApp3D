#include "flowControl.h"
#include "globalVars.h"
#include "objects.h"
#include "camera.h"
#include "input.h"
#include "globalScripts.h"
#include "progUI.h"

extern Camera _mainCamera;

Object* cube = nullptr;
Object* square = nullptr;
Object* textObj = nullptr;

TextBox* textTest = nullptr;


void progStart() {
				progUI::fps();

    setBgColor(0.2, 0.3, 0.3, 1);
    _windowTitle = "UI testing";

    cube = createObj("cube");
    cube->transform.position = { 0, 0, -2 };
				cube->color = { 0, 0, 1, 1 };

    square = createObj();
    square->UI = true;
    square->transform.scale = 0.25;
				square->color = { 1, 0, 0, 1 };

				textObj = createObj("square");
				textObj->UI = true;
				textObj->transform.scale = 0.25;
				textObj->transform.position.z = 0.05f;

				textTest = new TextBox();
				textTest->mode = TEXT_CENTER_RENDER;
				textTest->text = "Hello";
				textTest->fontSize = 0.4f;
				textTest->lineSize = 0.4f;
				textTest->fontColor = { 0, 1, 0, 1 };

				addObjScript(textObj, textTest);
}

void progUpdate() {
    if (keyAction::keyHeld(GLFW_KEY_SPACE)) {
        _mainCamera.position.z += 0.25 * _deltaTime;
    }

				if (keyAction::keyPressed(GLFW_KEY_A)) {
								setObjMesh(square, "square");
				}
}

void progEnd() {

}