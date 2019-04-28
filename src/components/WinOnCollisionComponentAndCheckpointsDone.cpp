#include <vector>
#include <ResourceManager.h>
#include <HudRenderer.h>
#include "WinOnCollisionComponentAndCheckpointsDone.h"
#include "../ui/WinMessage.h"
#include "GameObject.h"

WinOnCollisionComponentAndCheckpointsDone::WinOnCollisionComponentAndCheckpointsDone(std::shared_ptr<std::vector<bool>> checkpoints, std::shared_ptr<Scene> scene) {
    this->checkpoints = checkpoints;
    this->scene = scene;
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
        auto menuBg = ResourceManager::loadAndFetchTexture("../assets/menu/bubba_menu.png");

        std::shared_ptr<GameObject> hudObj = std::make_shared<GameObject>();
        auto winScreenRenderer = new HudRenderer();
        winScreenRenderer->setLayout(new WinMessage());

        hudObj->addRenderComponent(winScreenRenderer );
        scene->addTransparentObject(hudObj);
    }
}

void WinOnCollisionComponentAndCheckpointsDone::update(float dt) {

}
