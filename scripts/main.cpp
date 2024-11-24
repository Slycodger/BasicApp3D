#include "ProgUI.h"
#include "globalVars.h"
#include "flowControl.h"
#include "globalScripts.h"
#define STB_IMAGE_IMPLEMENTATION
#include "textureLoading.h"
#include "soundLoading.h"

Object* button = nullptr;
Button* buttonScr = nullptr;
Vec2 offset;

Object* dropDownFieldObj = nullptr;
DropDownFieldDynamic* dropDownScr = nullptr;

const char* imageLoc = "image.png";
unsigned int buttonTexture = 0;

const char* soundLoc = "sounds/sound.mp3";
const char* musicLoc = "sounds/funkyMusic.mp3";

sf::SoundBuffer* sb = nullptr;

Object* obj1 = nullptr;
Object* obj1Child = nullptr;
Object* obj2 = nullptr;
Button* obj1Button = nullptr;

Object* cube = nullptr;
Object* cube1 = nullptr;
Object* cube2 = nullptr;

float rotationSpeed = 20;

extern Camera MainCamera;

void onPressed(Button* button) {
    offset = { button->thisObj->transform.position.x - _mousePosX, button->thisObj->transform.position.y - _mousePosY };
}

void onHeld(Button* button) {
    button->thisObj->transform.position = offset + Vec2(_mousePosX, _mousePosY);
}

void progStart() {
    sb = new sf::SoundBuffer;
    sb->loadFromFile(soundLoc);

    setBgColor(0.2, 0.3, 0.3, 0);

    cube = createObj("cube");
    cube->transform.scale = { 0.5, 0.5, 0.5 };
    cube->transform.position = { 1, 0, -2 };
    cube->transform.rotation = { 0, 0, 0 };
    cube->color = {0.2, 0.8, 0.3, 1};

    cube1 = createObj("cube");
    cube1->transform.scale = { 0.5, 0.5, 0.5 };
    cube1->transform.position = { 1, 0, -4 };
    cube1->transform.rotation = { 0, 0, 0 };
    cube1->color = { 0.8, 0.2, 0.3, 1 };

    cube2 = createObj("cube");
    cube2->transform.scale = { 0.5, 0.5, 0.5 };
    cube2->transform.position = { 1, 0, -6 };
    cube2->transform.rotation = { 0, 0, 0 };
    cube2->color = { 0.2, 0.3, 1, 1 };

    button = createObj("square");
    button->transform.scale = { 0.5, 0.5, 1 };
    button->UI = true;
    buttonScr = new Button;

    int iWidth, iHeight = 0;

    if (loadTexture(imageLoc, buttonTexture, iWidth, iHeight)) {
        button->setTexture(buttonTexture);
        button->transform.scale.x = 0.5 * (float)iWidth / iHeight;
    }
    else
        std::cout << "Failed to load texture\n";

    buttonScr->BonPressed = onPressed;
    buttonScr->BonHeld = onHeld;


    addObjScript(button, buttonScr);

    button->color = { 0.9, 0.9, 0.9, 0.7 };
    buttonScr->textScr->fontColor = { 1, 1, 1, 1 };
    buttonScr->textScr->fontSize = 0.2;
    buttonScr->textScr->text = "Drag to move";
    buttonScr->textScr->textUpdate();


    dropDownFieldObj = createObj("square");

    dropDownFieldObj->transform.scale = { 0.3, 0.2, 1 };
    dropDownFieldObj->transform.position = { -_screenRatio + 0.3f, 1 - 0.2f, 0 };

    dropDownScr = new DropDownFieldDynamic;

    dropDownScr->buttonText.fontColor = { 0, 0, 0, 1 };
    dropDownScr->buttonText.mode = TEXT_CENTER_RENDER;
    dropDownScr->buttonText.fontSize = 0.3f;
    dropDownScr->buttonText.lineSize = 0.31f;

    addObjScript(dropDownFieldObj, dropDownScr);

    dropDownScr->textScr->text = "Yes or No";
    dropDownScr->textScr->textUpdate();
    dropDownScr->addOption("Option hey there");
    dropDownScr->addOption("Yes");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("No");
    dropDownScr->addOption("EA");
    dropDownScr->addOption("EA");
    dropDownScr->addOption("EA");
    dropDownScr->addOption("EA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("BA");
    dropDownScr->addOption("CA");

    obj1 = createObj("square");
    obj1Child = createObj("square");
    obj1Button = new Button;

    addObjScript(obj1, obj1Button);
    obj1->transform.scale = { 0.2, 0.2, 1 };
    obj1Child->transform.scale = { 0.1, 0.1, 1 };
    obj1Child->transform.position.y += 0.4;
    obj1Child->setParent(obj1);
    
    saveObj(obj1, "parentChildTest");

    obj2 = instantiateObj("parentChildTest");
    if (obj2)
        obj2->transform.position.x += 0.5;
}

int musicIndex = 0;
void progMain() {
    if (keyAction::keyPressed(GLFW_KEY_SPACE))
        progUI::fps();
    if (keyAction::keyPressed(GLFW_KEY_A))
        _vsync = !_vsync;

    if (keyAction::keyPressed(GLFW_KEY_B))
        dropDownScr->removeOption(0);

    if (keyAction::keyHeld(GLFW_KEY_UP))
        dropDownFieldObj->transform.position.y += 0.05 * _deltaTime;
    if (keyAction::keyHeld(GLFW_KEY_DOWN))
        dropDownFieldObj->transform.position.y -= 0.05 * _deltaTime;
    if (keyAction::keyHeld(GLFW_KEY_LEFT))
        dropDownFieldObj->transform.position.x -= 0.05 * _deltaTime;
    if (keyAction::keyHeld(GLFW_KEY_RIGHT))
        dropDownFieldObj->transform.position.x += 0.05 * _deltaTime;

    if (keyAction::keyPressed(GLFW_KEY_ENTER))
        playSound(*sb);

    if (keyAction::keyPressed(GLFW_KEY_KP_0))
        if (musicLoaded(musicIndex)) {
            if (!musicPlaying(musicIndex))
                startMusic(musicIndex);
        }
        else
            musicIndex = playMusic(musicLoc);

    if (keyAction::keyPressed(GLFW_KEY_KP_1))
        if (musicLoaded(musicIndex))
            pauseMusic(musicIndex);

    if (keyAction::keyPressed(GLFW_KEY_KP_2))
        if (musicLoaded(musicIndex))
            stopMusic(musicIndex);

    if (keyAction::keyHeld(GLFW_KEY_W))
        MainCamera.position += MainCamera.directionFacing() * 0.5f * _deltaTime;

    if (keyAction::keyHeld(GLFW_KEY_Q))
        MainCamera.addRotation(0, -90 * _deltaTime, 0);
    if (keyAction::keyHeld(GLFW_KEY_E))
        MainCamera.addRotation(0, 90 * _deltaTime, 0);

    if (_hideMouse) {
        MainCamera.setRotation(-_mousePosY * rotationSpeed, _mousePosX  * rotationSpeed, 0);
    }

    if (keyAction::keyPressed(GLFW_KEY_T))
        _hideMouse = !_hideMouse;

    if (keyAction::keyPressed(GLFW_KEY_Y))
        _showUI = !_showUI;
}

void progEnd() {
    glDeleteTextures(1, &buttonTexture);
    delete sb;
}