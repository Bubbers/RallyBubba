

#include <HudRenderer.h>
#include <FontManager.h>
#include <PositioningLayout.h>
#include <TextLayout.h>
#include <SFML/System/Clock.hpp>
#include "TimeDisplayComponent.h"
#include <sstream>
#include <iomanip>


TimeDisplayComponent::TimeDisplayComponent(std::shared_ptr<HudRenderer> hudRenderer, std::shared_ptr<sf::Clock> clock) {

    this->hudRenderer = hudRenderer;
    this->clock = clock;

    Font* ubuntuFont28 = FontManager::getInstance()->loadAndFetchFont("../fonts/Ubuntu-M.ttf", 28);

    auto timeHUD = new PositioningLayout(Dimension::fromPercentage(70.0f), Dimension::fromPercentage(70.0f));

    timeText = new TextLayout("Time: 0.00", ubuntuFont28, Dimension::fromPixels(200), Dimension::fromPixels(32));
    timeHUD->addChild(timeText, Dimension::fromPercentagePlusPixels(0.0f, 10), Dimension::fromPercentagePlusPixels(100.0f, -42));

    hudRenderer->setLayout(timeHUD);
}

void TimeDisplayComponent::update(float dt) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << clock->getElapsedTime().asSeconds();
    std::string timeAsString = stream.str();

    timeText->setText("Time: " + timeAsString);
    hudRenderer->updateLayout();
}
