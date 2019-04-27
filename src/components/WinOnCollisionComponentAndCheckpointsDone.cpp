#include <vector>
#include "WinOnCollisionComponentAndCheckpointsDone.h"

WinOnCollisionComponentAndCheckpointsDone::WinOnCollisionComponentAndCheckpointsDone(std::shared_ptr<std::vector<bool>> checkpoints) {
    this->checkpoints = checkpoints;
}

void WinOnCollisionComponentAndCheckpointsDone::beforeCollision(std::shared_ptr<GameObject> collider) {
    bool hasAllCheckpoints = true;
    for (bool b : (*checkpoints.get())) {
        if (!b) {
            hasAllCheckpoints = false;
            break;
        }
    }

    if (hasAllCheckpoints) {
        printf("WIN\n");
    }
}

void WinOnCollisionComponentAndCheckpointsDone::update(float dt) {

}
