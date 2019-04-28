//
// Created by simon on 2019-04-27.
//

#include <FontManager.h>
#include <ResourceManager.h>
#include "Menu.h"
#include "PlayButton.h"

Menu::Menu(Texture *background, std::function<void()> startCallback): PositioningLayout(Dimension::fromPercentage(100.0f), Dimension::fromPercentage(100.0f)) {
    auto graphic = new HUDGraphic(background);
    setBackground(graphic);
    addChild(new PlayButton(startCallback), Dimension::fromPercentagePlusPixels(50.0f, -100), Dimension::fromPercentagePlusPixels(50.0f, -20));

    std::string text = "Control the car with WASD. \r\nThe slower you drive, the sharper you turn. \r\nRespawn at last checkpoint with Backspace.\r\n"
                       "Minimize the time taken to race.";
    auto instructions = new TextLayout(text, FontManager::getInstance()->loadAndFetchFont("../assets/fonts/Ubuntu-M.ttf", 18), Dimension::fromPixels(400), Dimension::fromPixels(100));

    auto graphic2 = new HUDGraphic(ResourceManager::loadAndFetchTexture("../assets/meshes/alphablue.png").get());
    instructions->setBackground(graphic2);
    addChild(instructions, Dimension::fromPercentagePlusPixels(50.0f, -200), Dimension::fromPercentagePlusPixels(50.0f, 50));
}
