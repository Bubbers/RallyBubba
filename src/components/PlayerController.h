//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_PLAYEROBJECT_H
#define HALLSOFBUBBA_PLAYEROBJECT_H


#include <MoveComponent.h>

class PlayerController : public MoveComponent{

public:
    PlayerController(std::shared_ptr<std::vector<std::vector<char>>> tiles, float tileWidth);

    virtual void update(float dt);

private:
    chag::float3 up_vector = chag::make_vector(0.0f, 1.0f, 0.0f);
    float rotationSpeed = 0.03f;
    float maxSpeed = 10.0f;
    float minSpeed = -5.0f;
    float speed = 0.0f;
    float maxAcceleration = 10.0f;
    float minAcceleration = -10.0f;
    float acceleration = 0.0f;
    chag::float3 original_front_vector = chag::make_vector(0.0f, 0.0f, 1.0f);
    chag::float3 front_vector = original_front_vector;
    float passiveSlowdown = 2.0f;
    std::shared_ptr<std::vector<std::vector<char>>> tiles;
    float tileWidth;

    bool isOnAsphalt();
    float clamp(float value, float min, float max);
};


#endif //HALLSOFBUBBA_PLAYEROBJECT_H
