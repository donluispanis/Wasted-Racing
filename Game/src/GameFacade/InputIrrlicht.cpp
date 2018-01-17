#include "InputIrrlicht.h"
#include "../GameEvent/EventManager.h"
#include "../GameManager/RenderManager.h"
#include "../Game.h"

#include <iostream>

void InputIrrlicht::openInput(uintptr_t dev) {

    device = reinterpret_cast<irr::IrrlichtDevice*>(dev);

}

void InputIrrlicht::updateInput() { 

    if(!device->run()){
        EventManager::getInstance().addEvent(Event {EventType::Game_Close});
    }
        

}

void InputIrrlicht::closeInput() {
       
} 

bool InputIrrlicht::OnEvent(const irr::SEvent& event) {

    if(event.EventType == irr::EET_KEY_INPUT_EVENT){
        DetectKeyInput(KEY_SPACE, Key_Jump_Down, Key_Jump_Up)
        DetectKeyInput(KEY_ESCAPE,Key_Back_Down,Key_Back_Up)
        DetectKeyInput(KEY_KEY_W,Key_Advance_Down,Key_Advance_Up)
        DetectKeyInput(KEY_KEY_S,Key_Brake_Down,Key_Brake_Up)
        DetectKeyInput(KEY_KEY_A,Key_TurnLeft_Down,Key_TurnLeft_Up)
        DetectKeyInput(KEY_KEY_D,Key_TurnRight_Down,Key_TurnRight_Up)
        DetectKeyInput(KEY_KEY_T,Key_Drift_Down,Key_Drift_Up)
        DetectKeyInput(KEY_KEY_J,Key_Flanger_Down,Key_Flanger_Up)
        DetectKeyInput(KEY_KEY_K,Key_Decflanger_Down,Key_Decflanger_Up)
        DetectKeyInput(KEY_KEY_I,Key_ActivateA_Down,Key_ActivateA_Up)
        DetectKeyInput(KEY_KEY_O,Key_ActivateK_Down,Key_ActivateK_Up)
        DetectKeyInput(KEY_KEY_Q,Key_UseItem_Down,Key_UseItem_Up)
        DetectKeyInput(KEY_F9,Key_DebugAI_Down,Key_DebugAI_Up)
    }
    if(event.EventType == irr::EET_MOUSE_INPUT_EVENT){
        switch(event.MouseInput.Event)
        {
        case irr::EMIE_LMOUSE_PRESSED_DOWN:
            EventManager::getInstance().addEvent(Event {EventType::Key_Select_Down});
            break;
        case irr::EMIE_LMOUSE_LEFT_UP:
            EventManager::getInstance().addEvent(Event {EventType::Key_Select_Up});
            break;
        case irr::EMIE_RMOUSE_PRESSED_DOWN:
            EventManager::getInstance().addEvent(Event {EventType::Key_Back_Down});
            break;
        case irr::EMIE_RMOUSE_LEFT_UP:
            EventManager::getInstance().addEvent(Event {EventType::Key_Back_Up});
            break;
        case irr::EMIE_MOUSE_MOVED:
            MouseState.Position.X = event.MouseInput.X;
            MouseState.Position.Y = event.MouseInput.Y;
            break;
        default: break;
        }
    }
    return false;
}