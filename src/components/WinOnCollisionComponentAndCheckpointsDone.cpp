#include <vector>
#include <ResourceManager.h>
#include <HudRenderer.h>
#include "WinOnCollisionComponentAndCheckpointsDone.h"
#include "../ui/WinMessage.h"
#include "GameObject.h"

WinOnCollisionComponentAndCheckpointsDone::WinOnCollisionComponentAndCheckpointsDone(
        std::shared_ptr<std::vector<bool>> checkpoints,
        std::shared_ptr<Scene> scene,
        std::shared_ptr<sf::Clock> clock,
        std::function<void ()> func) {
    this->checkpoints = checkpoints;
    this->scene = scene;
    this->clock = clock;
    this->has_won = false;
    this->func = func;
}

void WinOnCollisionComponentAndCheckpointsDone::beforeCollision(std::shared_ptr<GameObject> collider) {
    if (has_won) {
        return;
    }

    bool hasAllCheckpoints = true;
    for (bool b : (*checkpoints.get())) {
        if (!b) {
            hasAllCheckpoints = false;
            break;
        }
    }

    if (!hasAllCheckpoints) {
        this->has_won = true;

        std::shared_ptr<GameObject> hudObj = std::make_shared<GameObject>();
        auto winScreenRenderer = new HudRenderer();
        winScreenRenderer->setLayout(new WinMessage(clock->getElapsedTime().asSeconds(), func));

        hudObj->addRenderComponent(winScreenRenderer );
        scene->addTransparentObject(hudObj);
    }
}

void WinOnCollisionComponentAndCheckpointsDone::update(float dt) {

}
