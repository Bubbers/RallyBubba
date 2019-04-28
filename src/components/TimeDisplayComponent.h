

#pragma once


#include <IComponent.h>

class TimeDisplayComponent: public IComponent {
public:
    TimeDisplayComponent(std::shared_ptr<HudRenderer> hudRenderer, std::shared_ptr<sf::Clock> clock);

    void update(float dt) override;

    std::shared_ptr<HudRenderer> hudRenderer;
    std::shared_ptr<sf::Clock> clock;
    TextLayout *timeText;
};



