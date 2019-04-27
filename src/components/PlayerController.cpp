//
// Created by simon on 2017-04-29.
//

#include <ControlsManager.h>
#include <controls.h>
#include "PlayerController.h"
#include "linmath/float3x3.h"

void PlayerController::update(float dt) {
    if (owner.expired()) {
        return;
    }

    std::shared_ptr<GameObject> owner_ptr = owner.lock();

    ControlsManager* cm = ControlsManager::getInstance();
    ControlStatus accelerationStatus = cm->getStatus(ACCELERATE);
    ControlStatus turnStatus = cm->getStatus(TURN);

    if (accelerationStatus.isActive()) {
        acceleration = acceleration + dt * accelerationStatus.getValue() / 100.0f;
        acceleration = clamp(acceleration, minAcceleration, maxAcceleration);
    } else {
        if(speed > 0) {
            acceleration = -passiveSlowdown * dt;
        } else if(speed < 0) {
            acceleration = passiveSlowdown * dt;
        } else {
            acceleration = 0.0f;
        }
    }
    speed = clamp(speed + acceleration, minSpeed, maxSpeed);
    setVelocity(front_vector * speed);

    if(turnStatus.isActive()){
        float angle = rotationSpeed * turnStatus.getValue();
        angle *= abs(speed/maxSpeed);
        if(speed < 0.0f) {
            angle *= -1;
        }
        setRotationSpeed(make_quaternion_axis_angle(up_vector, -angle));
    } else {
        setRotationSpeed(chag::Quaternion());
    }

    MoveComponent::update(dt);
    front_vector = toMatrix3x3(owner_ptr->getRelativeRotation()) * original_front_vector;
}

float PlayerController::clamp(float value, float min, float max) {
    if(value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}