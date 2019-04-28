#ifndef HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H
#define HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H


#include <IComponent.h>
#include <Scene.h>

class WinOnCollisionComponentAndCheckpointsDone: public IComponent {
public:
    WinOnCollisionComponentAndCheckpointsDone(std::shared_ptr<std::vector<bool>> checkpoints, std::shared_ptr<Scene> scene, std::shared_ptr<sf::Clock> clock, std::function<void ()> func);
    void beforeCollision(std::shared_ptr<GameObject> collider);
    void update(float dt);

private:
    std::shared_ptr<std::vector<bool>> checkpoints;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<sf::Clock> clock;
    bool has_won;
    std::function<void()> func;
    std::shared_ptr<sf::Sound> win_sound;
};


#endif //HALLSOFBUBBA_WINONCOLLISIONCOMPONENT_H
