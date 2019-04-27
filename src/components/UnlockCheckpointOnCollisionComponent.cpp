

#include "UnlockCheckpointOnCollisionComponent.h"
#include "GameObject.h"

UnlockCheckpointOnCollisionComponent::UnlockCheckpointOnCollisionComponent(int checkpointNumber, std::shared_ptr<std::vector<bool>> checkpoints) {
    this->checkpointNumber = checkpointNumber;
    this->checkpoints = checkpoints;
}

void UnlockCheckpointOnCollisionComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    (*this->checkpoints.get())[checkpointNumber] = true;
    this->owner.lock().get()->makeDirty();
}

void UnlockCheckpointOnCollisionComponent::update(float dt) {

}