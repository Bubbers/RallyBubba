//
// Created by simon on 2019-04-27.
//

#include "Menu.h"
#include "PlayButton.h"

Menu::Menu(Texture *background, std::function<void()> startCallback): PositioningLayout(Dimension::fromPercentage(100.0f), Dimension::fromPercentage(100.0f)) {
    auto graphic = new HUDGraphic(background);
    setBackground(graphic);
    addChild(new PlayButton(startCallback), Dimension::fromPercentagePlusPixels(50.0f, -100), Dimension::fromPercentagePlusPixels(50.0f, -20));
}
