#pragma once

#include "ISensorComponent.h"
#include "VObject.h"
#include <glm/ext.hpp>
#include <vector>

class VSensorComponent : public ISensorComponent{
    public:

        //Constructors
        VSensorComponent(GameObject& newGameObject);
        VSensorComponent(GameObject& newGameObject, float angV, float angI);
        
        //Destructor
        virtual ~VSensorComponent() {};



        //Checks the objects receives and stores the ones seen in the seenObjects vector
        void updateSeenObjects(GameObject** objects, int s);

        //Getters and setters
        std::vector<VObject*> getSeenObjects()           {   return seenObjects;    };
        glm::vec3 getSensorLeft()                        {   return sensorLeft;     };
        glm::vec3 getSensorRight()                       {   return sensorRight;    };

    private:
        //Angle of the sensor
        float angleInitial;
        //Angle of vision
        float angleVision;

        //Vectors that define the limit of the sensors
        glm::vec3 sensorLeft, sensorRight;

        //Objects being seen
        std::vector<VObject*> seenObjects;


};