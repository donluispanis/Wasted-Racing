#include "LAPALPhysics.h"
#include <math.h>
#include <iostream>

//Updates all linear variables 
void LAPAL::updateLinearVelocity(LAPAL::movementData& mData, const float dTime) {

    //The approach is that we define a base acceleration for every object and it does not change when it reaches the max acceleration
    //then, we calculate how the different forces affect the acceleration and, therefore, the velocity

    //BASE ACCELERATION
    mData.acc += mData.dAcc*dTime; //increment of acceleration * increment of time

    //If we aren't accelerating
    if(!mData.mov && mData.acc!=0) {
        if(abs(mData.acc)<0.1) {
            mData.acc = 0;
        }
    }

    //Check acceleration limits
    if(abs(mData.acc)>abs(mData.max_acc)){
        mData.acc = copysign(mData.max_acc, mData.acc);
    }

    //BASE VELOCITY
    mData.vel += mData.acc*dTime; 

    //If we aren't accelerating
    if(!mData.mov && mData.vel!=0) {
        if(abs(mData.vel)<0.1) {
            mData.vel= 0;
        }
    }

    //Check velocity limits 
    if(abs(mData.vel)>abs(mData.max_vel)){
        mData.vel = copysign(mData.max_vel, mData.vel);
    }
    
}

void LAPAL::updateAcceleration(LAPAL::movementData& mData, LAPAL::plane3f& terrain, const LAPAL::vec3f& position){

    //acc is base acceleration, acceleration is a vector, accDif is the acceleration diference applying the forces

    if(LAPAL::checkTerrain(terrain)){//if the plane is horizontal
        if(LAPAL::checkTerrainCollision(terrain, position)){//if the object is on an horizontal plane
        mData.finalAcc.x = (mData.acc + mData.accDif.x);
        mData.finalAcc.y = 0;
        mData.finalAcc.z = (mData.acc + mData.accDif.z);
        }
        else{//if the object is flying above an horizontal plane
        mData.finalAcc = mData.acc + mData.accDif;
        }
    }
    else{//if the object is not on an horizontal plane
        mData.finalAcc = mData.acc + mData.accDif;
    }

}

void LAPAL::updateVelocity(LAPAL::movementData& mData, LAPAL::plane3f& terrain){

    mData.velocity.x = mData.vel*cos(mData.angle);
    mData.velocity.y = mData.vel*sin(terrain.incAngle);
    mData.velocity.z = mData.vel*sin(mData.angle);
    
}

//Checks 2D collision between circles
bool LAPAL::checkCircleCircleCollision(const LAPAL::vec3f& pos1,const float& radius1, const LAPAL::vec3f& pos2,const float& radius2) {
    //if ( sqrt( ( pos2.x-pos1.x ) * ( pos2.x-pos1.x )  + ( pos2.z-pos1.z ) * ( pos2.z-pos1.z ) ) < ( radius1 + radius2 ) ) // square(x^2+z^2) < total radius
    if ( ( pos2.x-pos1.x ) * ( pos2.x-pos1.x )  + ( pos2.z-pos1.z ) * ( pos2.z-pos1.z ) < ( radius1 + radius2 ) * ( radius1 + radius2) ) // = (x² + z²)<tRadius²
        return true;
    return false;
} 

//Updates all spin related variables
void LAPAL::updateSpin(LAPAL::movementData& mData, const float dTime){

    if(mData.spi) {
        mData.spin += mData.vel*mData.spin_inc*dTime; //Spin depends on vel and spin_inc
    }
    else {
        if(abs(mData.spin) < 0.001) {
            mData.spin = 0;
        }
       /* else {
            mData.spin -= mData.spin*0.015;
        }*/
    }

    if(abs(mData.spin)>abs(mData.max_spin)){
        mData.spin = copysign(mData.max_spin, mData.spin);
    }

    mData.angle += mData.spin;

}

//Updates the velocity only in the components x and z
void LAPAL::update2DVelocity(LAPAL::movementData& mData) { //TO ERASE

}

//Assuming there's collision, changes velocity of every object after collision
void LAPAL::calculateElasticCollision(LAPAL::vec3f& vel1, float& mass1, LAPAL::vec3f& vel2, float& mass2) {

    //vel1 = vel1*(mass1-mass2)/massTotal + vel2*(mass2²)/(massTotal)
    //vel2 = vel1*(mass1²)/massTotal + vel2*(mass2-mass1)/(massTotal)

    vel1.x = vel1.x*(mass1 - mass2)/(mass1 + mass2) + vel2.x*(mass2 + mass2)/(mass1 + mass2);
    vel2.x = vel1.x*(mass1 + mass1)/(mass1 + mass2) + vel2.x*(mass2 - mass1)/(mass1 + mass2);

    vel1.z = vel1.z*(mass1 - mass2)/(mass1 + mass2) + vel2.z*(mass2 + mass2)/(mass1 + mass2);
    vel2.z = vel1.z*(mass1 + mass1)/(mass1 + mass2) + vel2.z*(mass2 - mass1)/(mass1 + mass2);

}

//Calculates a plane's normal
LAPAL::vec3f LAPAL::calculateNormal(const LAPAL::plane3f& plane){
    
    LAPAL::vec3f u; //vector a of the plane
    LAPAL::vec3f v; //vector b of the plane
    LAPAL::vec3f normal;

    //To calculate the vectors of the plane, we only need 3 of it's point coordenates
    u.x = plane.p1.x - plane.p3.x;
    u.y = plane.p1.y - plane.p3.y;
    u.z = plane.p1.z - plane.p3.z;

    v.x = plane.p2.x - plane.p3.x;
    v.y = plane.p2.y - plane.p3.y;
    v.z = plane.p2.z - plane.p3.z;


    //The normal of the plane is the cross product of vectors a and b with components i,j,k
    normal.x = (u.y*v.z) - (u.y*v.z); //component i
    normal.y = (v.x*u.z) - (u.x*v.z); //component j
    normal.z = (v.x*u.y) - (u.x*v.y); //component k

    return normal;

}

//Checks if there's a collision between terrain and object
bool LAPAL::checkTerrainCollision(const LAPAL::plane3f& terrain,const LAPAL::vec3f& position){
    
    bool collision=false;
    LAPAL::vec3f u = calculateNormal(terrain);
    float a, b, c, d, equ; //coeficients of the implicit equation Ax + By + Cz + D = 0

    //First we need to get the implicit equation of the plane, we need two vectors and a point of the plane
    /*u.x = terrain.p1.x - terrain.p3.x;
    u = terrain.p1 - terrain.p3;
    v = terrain.p2 - terrain.p3;
    v.y = terrain.p2.y - terrain.p3.y;
    v.z = terrain.p2.z - terrain.p3.z;
    //Calculating the equation coeficients
    a = (u.y*v.z) - (u.z*v.y);
    b = (u.x*v.z) - (u.z*v.x);
    c = (u.x*v.y) - (u.x*v.y);
    d = u.x*(-terrain.p1.x) + u.y*(-terrain.p1.y) + u.z*(-terrain.p1.z);*/

    //To see if there's a collision, the point (the position) of the object must be inside the plane, therefore the equation with that point must be 0
    //equ = u.x*(position.x) + u.y*(position.y) + u.z*(position.z)+d;
    equ = u.x*(position.x-terrain.p1.x) + u.y*(position.y-terrain.p1.y) + u.z*(position.z-terrain.p1.z); // 

    if (equ == 0){
        collision=true;
    }

    return collision;

}

void LAPAL::correctTerrainCollision(LAPAL::plane3f& terrain, LAPAL::vec3f& position){

    //If the plane is parallel to the 2d plane xz, y will be the same for all points inside the plane
    if(LAPAL::checkTerrain(terrain)){
        
        position.y=terrain.p1.y;

    }
    else{//If the plane is inclined, we need to solve again the implicit plane equation for an Y inside the plane

        LAPAL::vec3f u;
        LAPAL::vec3f v;
        float a, b, c, d, y; //coeficients of the implicit equation Ax + By + Cz + D = 0

        //We need two vectors and a point of the plane
        u.x = terrain.p1.x - terrain.p3.x;
        u.y = terrain.p1.y - terrain.p3.y;
        u.z = terrain.p1.z - terrain.p3.z;

        v.x = terrain.p2.x - terrain.p3.x;
        v.y = terrain.p2.y - terrain.p3.y;
        v.z = terrain.p2.z - terrain.p3.z;

        //Calculating the equation coeficients
        a = (u.y*v.z) - (u.z*v.y);
        b = (u.x*v.z) - (u.z*v.x);
        c = (u.x*v.y) - (u.x*v.y);
        d = a*(-terrain.p1.x) + b*(-terrain.p1.y) + c*(-terrain.p1.z);

        //To see if there's a collision, the point (the position) of the object must be inside the plane, therefore the equation with that point must be 0
        //We need to see which number must be Y to make the equation = 0
        //If the implicit equation is Ax + By + Cz + D = 0, y = (-Ax-Cz-D)/B 

        y = (-a*(position.x) -c*(position.z) -d)/b;
        position.y=y;

    }
}

bool LAPAL::checkTerrain(LAPAL::plane3f& terrain){

    bool state=false;

   if(terrain.p1.y == terrain.p2.y && terrain.p1.y == terrain.p3.y && terrain.p1.y == terrain.p4.y){ 
        state=true; //the plane is horizontal
    }

    return state;
}

//Updates the friction force based on the terrain (force is a vector but we treat it like a variable and we will treat the different 3d components with it)
void LAPAL::updateFrictionForce(LAPAL::movementData& mData, LAPAL::plane3f& terrain, float& objMass, const float& gravity, const LAPAL::vec3f& position){

    //Friction force: F=μ*N

    vec3f u; //unitary vector
   
    //there's no friction on component y
    
        if(LAPAL::checkTerrainCollision(terrain, position)){ //the object is in the terrain
            if(LAPAL::checkTerrain(terrain)){//the terrain is horizontal
                mData.frictionForce.x = terrain.fric * objMass * gravity;
                mData.frictionForce.y = 0;
                mData.frictionForce.z = terrain.fric * objMass * gravity;
            }
            else{
                mData.frictionForce.x = terrain.fric * objMass * gravity;
                mData.frictionForce.y = terrain.fric * objMass * gravity;
                mData.frictionForce.z = terrain.fric * objMass * gravity;
            }
        }
        else{ //the object is in the air, in the air there's no friction
        mData.frictionForce.x = 0;
        mData.frictionForce.y = 0;
        mData.frictionForce.z = 0;
        }

    u = mData.velocity/mData.vel;
    mData.frictionForce = mData.frictionForce * (-u);
}

//Sums all forces
void LAPAL::updateGravityForce(LAPAL::movementData& mData, float& objMass, const float& gravity, LAPAL::plane3f& terrain, const LAPAL::vec3f& position){

    if(LAPAL::checkTerrain(terrain)){//If the terrain is horizontal, we don't have to calculate with 
       if(LAPAL::checkTerrainCollision(terrain, position)){ //the object is in the terrain
        mData.gravityForce.x = 0;
        mData.gravityForce.y = 0;
        mData.gravityForce.z = 0;
       }
       else{ //the object is in the air
        mData.gravityForce.x = 0;
        mData.gravityForce.y = (-1)*objMass*gravity;
        mData.gravityForce.z = 0;
       }
    }
    else{
        mData.gravityForce.x = ((-1)*objMass*gravity) * sin(terrain.incAngle) * cos(terrain.incAngle) * cos(terrain.rotAngle);
        mData.gravityForce.y = ((-1)*objMass*gravity) * sin(terrain.incAngle) * sin(terrain.incAngle);
        mData.gravityForce.z = ((-1)*objMass*gravity) * sin(terrain.incAngle) * cos(terrain.incAngle) * sin(terrain.rotAngle);
    }

}

//Updates the net force (the total of forces applied on the object without having in mind the push force)
void LAPAL::calculateNetForce(LAPAL::movementData& mData){

    mData.netForce.x = mData.frictionForce.x + mData.gravityForce.x;
    mData.netForce.y = mData.frictionForce.y + mData.gravityForce.y;
    mData.netForce.z = mData.frictionForce.z + mData.gravityForce.z;

}

//Updates the difference of acceleration
void LAPAL::updateAccDif(LAPAL::movementData& mData, float& objMass){

    mData.accDif.x = mData.netForce.x;
    mData.accDif.y = mData.netForce.y;
    mData.accDif.z = mData.netForce.z;

}

//Updates the difference of velocity
void LAPAL::updateVelDif(LAPAL::movementData& mData, const float& dTime){ //TO ERASE
/*
    if(mData.dAcc <= 0){
        mData.velDif.x = (-1)*mData.accDif.x*dTime;
        mData.velDif.y = (-1)*mData.accDif.y*dTime;
        mData.velDif.z = (-1)*mData.accDif.z*dTime;
    }
    else{
        mData.velDif.x = mData.accDif.x*dTime;
        mData.velDif.y = mData.accDif.y*dTime;
        mData.velDif.z = mData.accDif.z*dTime;

    }*/

}

//Updates the velocity in a 3D world
void LAPAL::update3DVelocity(LAPAL::movementData& mData, const float dTime){

        mData.vel3d = mData.finalAcc * dTime;

}

//Calculates the distance between a line defined by two points (l1,l2) and a point (p1)
float LAPAL::distance2DLinePoint(const LAPAL::vec3f& l1, const LAPAL::vec3f& l2, const LAPAL::vec3f& p1) {

    LAPAL::vec3f lineVec = l2-l1;
    LAPAL::vec3f circVec = l2-p1;

    float lineVecMod = sqrt(lineVec.x*lineVec.x + lineVec.z*lineVec.z);
    float circVecMod = sqrt(circVec.x*circVec.x + circVec.z*circVec.z);

    float vectorCos = (lineVec.x*circVec.x + lineVec.z*circVec.z)/(lineVecMod*circVecMod);

    float distance = vectorCos*circVecMod;

    return distance;

}

