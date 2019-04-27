

#include "UnlockCheckpointOnCollisionComponent.h"

UnlockCheckpointOnCollisionComponent::UnlockCheckpointOnCollisionComponent(int checkpointNumber, std::shared_ptr<std::vector<bool>> checkpoints) {
    this->checkpointNumber = checkpointNumber;
    this->checkpoints = checkpoints;
}

void UnlockCheckpointOnCollisionComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    (*this->checkpoints.get())[checkpointNumber] = true;
}

void UnlockCheckpointOnCollisionComponent::update(float dt) {

}