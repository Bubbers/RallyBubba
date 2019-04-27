#pragma once

#include <IComponent.h>
#include <vector>

class UnlockCheckpointOnCollisionComponent: public IComponent {
public:
    UnlockCheckpointOnCollisionComponent(int checkpointNumber, std::shared_ptr<std::vector<bool>> checkpoints);
    void beforeCollision(std::shared_ptr<GameObject> collider);
    void update(float dt);

private:
    int checkpointNumber;
    std::shared_ptr<std::vector<bool>> checkpoints;
};



