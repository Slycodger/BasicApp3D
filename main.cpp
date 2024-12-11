#include "flowControl.h"
#include "globalVars.h"
#include "objects.h"
#include "camera.h"
#include "input.h"

extern Camera _mainCamera;

Object* cube = nullptr;
Object* square = nullptr;



void progStart() {
    setBgColor(0.2, 0.3, 0.3, 1);
    _windowTitle = "UI testing";

    cube = createObj("cube");
    cube->transform.position = { 0, 0, -2 };

    square = createObj("square");
    square->UI = true;
    square->transform.scale = 0.25;
    square->color = { 1, 0, 0, 1 };
}

void progUpdate() {
    if (keyAction::keyHeld(GLFW_KEY_SPACE)) {
        _mainCamera.position.z += 0.25 * _deltaTime;
    }
}

void progEnd() {

}