//
// Created by simon on 2017-04-29.
//

#include <ControlsManager.h>
#include <controls.h>
#include <ResourceManager.h>
#include "PlayerController.h"
#include "linmath/float3x3.h"
#include "RallyConstants.h"

PlayerController::PlayerController(std::shared_ptr<std::vector<std::vector<char>>> tiles, float tileWidth, chag::float3 startPosition) {
    this->tiles = tiles;
    this->tileWidth = tileWidth;
    this->locationAtLastCheckpoint = startPosition;
    this->revvingSound = std::shared_ptr<sf::Sound>(ResourceManager::loadAndFetchSound("../assets/sounds/engine_revving.wav"));
    this->revvingSound->setVolume(50.0f);
    this->engineSound = std::shared_ptr<sf::Sound>(ResourceManager::loadAndFetchSound("../assets/sounds/engine_sound.wav"));
}

void PlayerController::beforeCollision(std::shared_ptr<GameObject> collider) {
    if (collider->getIdentifier() == COLLIDABLE_BUT_NO_COLLISION_IDENTIFIER) {
        locationAtLastCheckpoint = collider->getAbsoluteLocation();
    }
    if (collider->getIdentifier() == COLLIDABLE_AND_COLLISION_IDENTIFIER) {
        speed = 0;
        std::shared_ptr<GameObject> owner_ptr = owner.lock();
        owner_ptr->setLocation(locationAtLastUpdate);
    }
}

void PlayerController::duringCollision(std::shared_ptr<GameObject> collider) {
    if (collider->getIdentifier() == COLLIDABLE_AND_COLLISION_IDENTIFIER) {
        speed = 0;
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
    ControlStatus respawnStatus = cm->getStatus(RESPAWN);
    if (respawnStatus.isActive()) {
        owner_ptr->setLocation(locationAtLastCheckpoint);
        speed = 0.0f;
        return;
    }

    float acceleration;
    if (accelerationStatus.isActive()) {
        bool isAccelerating = false;
        float accelerationMultiplier;
        float accelerationAmount = accelerationStatus.getValue();
        if (speed > 0.0f) {
            if(accelerationAmount < 0.0f) {
                accelerationMultiplier = breakAcceleration;
            } else {
                accelerationMultiplier = maxAcceleration;
                isAccelerating = true;
            }
        } else if(speed < 0.0f) {
            if (accelerationAmount < 0.0f) {
                accelerationMultiplier = minAcceleration;
                isAccelerating = true;
            } else {
                accelerationMultiplier = breakAcceleration;
            }
        } else {
            isAccelerating = true;
            if (accelerationAmount < 0.0f) {
                accelerationMultiplier = minAcceleration;
            } else {
                accelerationMultiplier = maxAcceleration;
            }
        }
        if(isAccelerating) {
            if (revvingSound->getStatus() != sf::Sound::Playing) {
                engineSound->stop();
                revvingSound->play();
            }
        }
        acceleration = accelerationMultiplier * dt * accelerationAmount / 100.0f;
    } else {
        if (engineSound->getStatus() != sf::Sound::Playing) {
            revvingSound->stop();
            engineSound->play();
        }
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