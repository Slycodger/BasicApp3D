#define _PROG_UI
#include "progUI.h"

Object* UIFPSBG = nullptr;
Object* UIFPSObj = nullptr;
TextBox* UIFPSScr = nullptr;

namespace ProgUI {
    void start() {
								UIFPSBG = createObj("square");
								UIFPSBG->transform.scale = { 0.3, 0.075, 1 };
								UIFPSBG->transform.position = { _screenRatio - UIFPSBG->transform.scale.x, 1 - UIFPSBG->transform.scale.y, 1 };
								UIFPSBG->transform.position.z = 0.85f;
								UIFPSBG->active = false;
								UIFPSBG->UI = true;

        UIFPSObj = createObj("square");
        UIFPSScr = new TextBox;
        UIFPSObj->transform.scale = { 0.3, 0.075, 1 };
        UIFPSObj->transform.position = { _screenRatio - UIFPSObj->transform.scale.x, 1 - UIFPSObj->transform.scale.y, 1 };
        UIFPSObj->transform.position.z = 0.9f;
								UIFPSObj->UI = true;

        UIFPSScr->text = "FPS : ";
        UIFPSScr->font = "CascadiaCode_BIG";
        UIFPSScr->fontSize = 1;
        addObjScript(UIFPSObj, UIFPSScr);

								UIFPSObj->active = false;
				}
    void update() {
        if (UIFPSObj->active) {
            UIFPSScr->text = "FPS : " + std::to_string((int)(1 / _deltaTime));
            UIFPSScr->textUpdate();
        }
    }
}

namespace progUI{
    void fps(bool val) {
        UIFPSObj->active = val;
        UIFPSBG->active = val;
    }
    void fps() {
        UIFPSObj->active = !UIFPSObj->active;
								UIFPSBG->active = !UIFPSBG->active;
				}
}