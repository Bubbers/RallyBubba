//
// Created by simon on 2017-04-29.
//

#include <ControlsManager.h>
#include <controls.h>
#include "PlayerController.h"
#include "linmath/float3x3.h"
#include "RallyConstants.h"

PlayerController::PlayerController(std::shared_ptr<std::vector<std::vector<char>>> tiles, float tileWidth) {
    this->tiles = tiles;
    this->tileWidth = tileWidth;
}

void PlayerController::beforeCollision(std::shared_ptr<GameObject> collider) {
    if (collider->getIdentifier() == COLLIDABLE_AND_COLLISION_IDENTIFIER) {
        speed = 0;
        acceleration = 0;
        std::shared_ptr<GameObject> owner_ptr = owner.lock();
        owner_ptr->setLocation(locationAtLastUpdate);
    }
}

void PlayerController::duringCollision(std::shared_ptr<GameObject> collider) {
    if (collider->getIdentifier() == COLLIDABLE_AND_COLLISION_IDENTIFIER) {
        speed = 0;
        acceleration = 0;
        std::shared_ptr<GameObject> owner_ptr = owner.lock();
        owner_ptr->setLocation(locationAtLastUpdate);
    }
}

void PlayerController::update(float dt) {
    if (owner.expired()) {
        return;
    }

    std::shared_ptr<GameObject> owner_ptr = owner.lock();
    locationAtLastUpdate = owner_ptr->getRelativeLocation();

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
                acceleration = fmin(acceleration, -(fabs(speed) - (maxGrassSpeed - 0.5f)) * dt);
            } else {
                acceleration = fmax(acceleration, (fabs(speed) - (maxGrassSpeed - 0.5f)) * dt);
            }
        }
    }

    speed = clamp(speed + acceleration, minSpeed, maxSpeed);
    setVelocity(front_vector * speed);

    if(turnStatus.isActive()){
        float angle = getRotationMultiplier(fabs(speed)) * turnStatus.getValue();
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

    char currentTile = (*tiles.get())[y][x];

    return currentTile == '#';
}

// Sharper turn curve at lower speeds
float PlayerController::getRotationMultiplier(float speed) {
    return -speed/500.0f + 0.06f;
}