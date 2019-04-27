//
// Created by simon on 2019-04-27.
//

#include "PlayButton.h"
#include <HUDGraphic.h>
#include <FontManager.h>

PlayButton::PlayButton(std::function<void()> startCallback): TextLayout("PLAY", FontManager::getInstance()->loadAndFetchFont("../assets/fonts/Ubuntu-M.ttf", 25), Dimension::fromPixels(200), Dimension::fromPixels(40)) {
    setPadding(2);
    HUDGraphic::Color normalColor = HUDGraphic::Color("#6b512b");
    HUDGraphic::Color hoverColor = HUDGraphic::Color("#352714");
    auto graphic = new HUDGraphic(normalColor);
    graphic->setBorder(2, HUDGraphic::Color("#8d001c"));
    setBackground(graphic);
    addHoverListener([normalColor,hoverColor](int x, int y, Layout* element, bool enteredElseLeaving){
        auto color = enteredElseLeaving ? hoverColor : normalColor;
        element->getGraphic()->setBackground(color);
    });
    addClickListener([startCallback](int x, int y, Layout* element, bool enteredElseLeaving){
        startCallback();
    });
}