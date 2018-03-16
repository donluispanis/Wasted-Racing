#include "ScoreEvents.h"
#include "../GameObject/PhysicsComponent/MoveComponent.h"

//===========================================================
//      FACTORY REGISTER
//===========================================================
REGISTER_FACTORY(OnNewLapEvent);
REGISTER_FACTORY(OnOvertakeEvent);
REGISTER_FACTORY(OnOvertakenEvent);

//===========================================================
//      ON NEW LAP EVENT
//===========================================================
void OnNewLapEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    int player = std::dynamic_pointer_cast<MoveComponent>(event.Component).get()->getMovemententData().player;
    std::string name = "OnNewLap"+std::to_string(event.Component->getGameObject().getId());

    if(player >= 0 && player <= 4 && !audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

        //Set sound parameters
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (float)player);

        //If event ID is 0, we cross the line for the first or second time
        if(event.Id == 0)
            FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)2);
        //If event ID is 1, we cross the line for the third time and we are in one the first 3 positions
        if(event.Id == 1)
            FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)7);
        //If event ID is 2, we cross the line for the third time and we are in one of the last positions
        if(event.Id == 2)
            FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)8);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);
    }

}

//===========================================================
//      ON OVERTAKE EVENT
//===========================================================
void OnOvertakeEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    int player = std::dynamic_pointer_cast<MoveComponent>(event.Component).get()->getMovemententData().player;
    std::string name = "OnOvertake"+std::to_string(event.Component->getGameObject().getId());

    if(player >= 0 && player <= 4 && !audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

        //Set sound parameters
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (float)player);
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)1);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);
    }

}

//===========================================================
//      ON OVERTAKEN EVENT
//===========================================================
void OnOvertakenEvent::initalizeSound(AudioFMOD* audioFMOD, const EventData& event) {

    int player = std::dynamic_pointer_cast<MoveComponent>(event.Component).get()->getMovemententData().player;
    std::string name = "OnOvertaken"+std::to_string(event.Component->getGameObject().getId());

    if(player >= 0 && player <= 4 && !audioFMOD->existsSoundEvent(name)){

        //Load sound event
        FMOD_Studio_EventDescription_CreateInstance(audioFMOD->getEventDescriptions()["CharacterES"], &soundInstance);

        //Set sound parameters
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "player", (float)player);
        FMOD_Studio_EventInstance_SetParameterValue(soundInstance, "track", (float)3);

        //Set sound position
        glm::vec3 pos = event.Component.get()->getGameObject().getTransformData().position;

        FMOD_3D_ATTRIBUTES attributes;
        attributes.position.x = pos.x * audioFMOD->getWorldUnits();
        attributes.position.y = pos.y * audioFMOD->getWorldUnits();
        attributes.position.z = pos.z * audioFMOD->getWorldUnits();

        FMOD_Studio_EventInstance_Set3DAttributes(soundInstance, &attributes);

        //Start sound
        FMOD_Studio_EventInstance_Start(soundInstance);

        //Save references to the sound and the sound emitter
        emitter = std::weak_ptr<IComponent>(event.Component);
        audioFMOD->insertSoundEvent(name, (ISoundEvent*)this);
    }

}