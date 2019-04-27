//
// Created by simon on 2017-04-29.
//

#include <ControlsManager.h>
#include <controls.h>
#include "PlayerController.h"
#include "linmath/float3x3.h"

PlayerController::PlayerController(std::shared_ptr<std::vector<std::vector<int>>> tiles, float tileWidth) {
    this->tiles = tiles;
    this->tileWidth = tileWidth;
}

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

    if (!isOnAsphalt()) {
        float maxGrassSpeed = 5.0f;
        if (fabs(speed) > maxGrassSpeed) {
            if(speed > 0) {
                acceleration = -(float)fmin((fabs(speed) - maxGrassSpeed), 5.0) * dt;
            } else {
                acceleration = (float)fmin((fabs(speed) - maxGrassSpeed), 5.0) * dt;
            }
        }
    }

    speed = clamp(speed + acceleration, minSpeed, maxSpeed);
    setVelocity(front_vector * speed);

    if(turnStatus.isActive()){
        float angle = rotationSpeed * turnStatus.getValue();
        angle *= fabs(speed/maxSpeed);
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

bool PlayerController::isOnAsphalt() {
    const chag::float3 &absoluteLocation = owner.lock()->getAbsoluteLocation();

    int x = (int)(-absoluteLocation.x / tileWidth);
    int y = (int)(-absoluteLocation.z / tileWidth);

    int currentTile = (*tiles.get())[y][x];

    return currentTile == 1;
}
