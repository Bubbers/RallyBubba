//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_PLAYEROBJECT_H
#define HALLSOFBUBBA_PLAYEROBJECT_H


#include <MoveComponent.h>

class PlayerController : public MoveComponent{

public:
    PlayerController(std::shared_ptr<std::vector<std::vector<char>>> tiles, float tileWidth, chag::float3 startPosition);

    virtual void update(float dt);

    void duringCollision(std::shared_ptr<GameObject> collider) override;

    void beforeCollision(std::shared_ptr<GameObject> collider) override;

private:
    chag::float3 up_vector = chag::make_vector(0.0f, 1.0f, 0.0f);
    float maxSpeed = 20.0f;
    float minSpeed = -8.0f;
    float speed = 0.0f;
    float maxAcceleration = 10.0f;
    float minAcceleration = 10.0f;
    float breakAcceleration = 15.0f;
    chag::float3 original_front_vector = chag::make_vector(0.0f, 0.0f, 1.0f);
    chag::float3 front_vector = original_front_vector;
    float passiveSlowdown = 2.0f;
    std::shared_ptr<std::vector<std::vector<char>>> tiles;
    float tileWidth;
    chag::float3 locationAtLastUpdate;
    chag::float3 locationAtLastCheckpoint;

    bool isOnAsphalt();

    float clamp(float value, float min, float max);

    float getRotationMultiplier(float speed);
};


#endif //HALLSOFBUBBA_PLAYEROBJECT_H
