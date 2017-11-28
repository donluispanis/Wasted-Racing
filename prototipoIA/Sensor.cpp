#include "Sensor.h"

//Constructors
Sensor::Sensor(glm::vec3 p, double ang, float maxR, double angInit)
{
    position = p;
    angleVision = ang;
    maxRadius = maxR;
    angleInitial = angInit;
}
    
Sensor::Sensor()
{
    position=glm::vec3(0,0,0);
    angleVision=0;
    angleInitial=0;
    maxRadius=0;
    a=0;
    b=0;
}

//Destructor
Sensor::~Sensor()
{

}

//Detect if point is in the field of view
bool Sensor::detectFieldVision(glm::vec3 vel, glm::vec3 p){
    //First check the distance (avoiding squared root)
    if((p.x-position.x)*(p.x-position.x) + (p.z-position.z)*(p.z-position.z) > maxRadius*maxRadius)
    {
        return false;
    }
    
    //Calculate field of view left and right vectores, given the angle apart of the velocity vector and the position of the object
    irr::core::vector3df sensorLeft =irr::core::vector3df(sin(-angleVision-angleInitial), 0.f, cos(-angleVision-angleInitial));
    irr::core::vector3df sensorRight =irr::core::vector3df(sin(angleVision-angleInitial), 0.f, cos(angleVision-angleInitial));
    glm::vec3 relativeP = p - position;

    //calculate a and b coefficients, to compose the point
    if(sensorRight.X*sensorLeft.Z != sensorRight.Z*sensorLeft.X) 
        b = (relativeP.z * sensorLeft.X - relativeP.x*sensorLeft.Z) /(sensorRight.Z * sensorLeft.X - sensorRight.X*sensorLeft.Z);
    if(sensorLeft.X != 0){
        a = (relativeP.z - b * sensorRight.Z) / sensorLeft.Z;
    }

    std::cout << "B:" << b << std::endl;
    std::cout << "A:" << a << std::endl;    

    //TESTING COUTS
    //#####################################################################################################################
    //std::cout<<"distancia jeje: "<<glm::sqrt((p.X-position.X)*(p.X-position.X) 
    //						+ (p.Z-position.Z)*(p.Z-position.Z))<<std::endl;
    //std::cout<<"vectores: "<<sensorLeft.x<<","<<sensorLeft.z<<" - "<<sensorRight.x<<","<<sensorRight.z<<std::endl;
    //std::cout<<"A y B: "<<a<<","<<b<<std::endl;
    //Analyze conditions to not be in sensor field of viewanglevision
    //#####################################################################################################################

    if(a < 0 || b < 0) return false;
    
    return true;
}

VObject** Sensor::getAllFieldVisionObjects(irr::scene::ISceneNode** list, int size)
{
    VObject** objects=new VObject*[size];
    irr::core::vector3df sensorLeft =irr::core::vector3df(sin(-angleVision-angleInitial), 0.f, cos(-angleVision-angleInitial));
    irr::core::vector3df sensorRight =irr::core::vector3df(sin(angleVision-angleInitial), 0.f, cos(angleVision-angleInitial));
    glm::vec3 p;
    int i, j=0;
    glm::vec3 relativeP;


    for(i=0; i<size; i++)
    {
        if(list[i]!=NULL)
        {
            p = glm::vec3(list[i]->getPosition().X, list[i]->getPosition().Y, list[i]->getPosition().Z);
            if(!((p.x-position.x)*(p.x-position.x) + (p.z-position.z)*(p.z-position.z) > maxRadius*maxRadius))
            {
                relativeP = p - position;
                if(sensorRight.X*sensorLeft.Z != sensorRight.Z*sensorLeft.X) 
                    b = (relativeP.z * sensorLeft.X - relativeP.x*sensorLeft.Z) /(sensorRight.Z * sensorLeft.X - sensorRight.X*sensorLeft.Z);
                if(sensorLeft.X != 0){
                    a = (relativeP.z - b * sensorRight.Z) / sensorLeft.Z;
                }

                if(a >= 0 && b >= 0)
                {
                    objects[j]=new VObject(p, a, b, 1.f, 1);
                    j++;
                }
            }
        }
    }

    return objects;
}

void Sensor::updatePosition(glm::vec3 pos){
    position = pos;
}

void Sensor::updateAngle(double angle){
    angleInitial += angle;
}

irr::core::vector3df Sensor::getSensorLeft()
{
    irr::core::vector3df sensorLeft =irr::core::vector3df(sin(-angleVision-angleInitial), 0.f, cos(-angleVision-angleInitial));
    return sensorLeft;
}

irr::core::vector3df Sensor::getSensorRight()
{
    irr::core::vector3df sensorRight =irr::core::vector3df(sin(angleVision-angleInitial), 0.f, cos(angleVision-angleInitial));
    return sensorRight;
}