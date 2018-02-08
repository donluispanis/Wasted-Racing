#pragma once

#include "IGameState.h"
#include "../Game.h"
#include "../GameObject/GameObject.h"
#include "../GameObject/RenderComponent/ObjectRenderComponent.h"
#include "../GameObject/InputComponent.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"
#include "../GameObject/PhysicsComponent/RampComponent.h"
#include "../GameObject/ItemComponent/ItemBoxComponent.h"
#include "../GameObject/AIComponent/WaypointComponent.h"
#include "../GameManager/InputManager.h"
#include "../GameManager/RenderManager.h"
#include "../GameManager/ObjectManager.h"
#include "../GameManager/PhysicsManager.h"
#include "../GameManager/WaypointManager.h"
#include "../GameManager/AIManager.h"
#include "../GameManager/SensorManager.h"
#include "../GameManager/AudioManager.h"
#include "../GameManager/ScoreManager.h"
#include "../GameManager/ItemManager.h"
#include "../GameManager/NetworkManager.h"
#include "../GameManager/DebugManager.h"
#include "../GameEvent/EventManager.h"
#include "../GameFacade/AudioFMOD.h"

#include <memory>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>

class Game;
class NetworkManager;
class ObjectManager;
class ItemManager;
class SensorManager;
class AIManager;
class DebugManager;

class MultiMatchState : public IGameState {

public: 

    //Constructor
    MultiMatchState (){ type = IGameState::MULTIMATCH; };

    //Destructor
    virtual ~MultiMatchState() {}

    //Initialization
    virtual void init();

    //Updater
    virtual void update(float &accumulatedTime);

    //Update managers
    void updateManagers(float dTime);

    //Drawer
    virtual void draw();

    //Shutdown
    virtual void close();

    //Static class getter
    static MultiMatchState& getInstance() {
        static MultiMatchState instance;
        return instance;
    };

private:
    
    //==============================================================
    // Private data
    //==============================================================
    //Object manager
    ObjectManager* objectManager;
    //Input manager
    InputManager* inputManager;
    //Render manager
    RenderManager* renderManager;
    //Event manager
    EventManager* eventManager;
    //Physics manager
    PhysicsManager* physicsManager;
    //Waypoint manager
    WaypointManager* waypointManager;
    //Audio Manager
    AudioManager* audioManager;
    //AI manager
    AIManager* aiManager;
    //Sensor manager
    SensorManager* sensorManager;
    //Item manager
    ItemManager* itemManager;
    //Score manager
    ScoreManager* scoreManager;
    //Network manager
    NetworkManager* networkManager;
    //Debug manager
    DebugManager* debugManager;

    //Update's loop time
    const float loopTime = 1.0f/30.0f;

};