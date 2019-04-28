#include <Dimension.h>
#include <TextLayout.h>
#include <FontManager.h>
#include <ResourceManager.h>
#include <ListLayout.h>
#include "WinMessage.h"
#include "HUDGraphic.h"

WinMessage::WinMessage(float elapsedTimeSeconds): PositioningLayout(Dimension::fromPercentage(100.0f), Dimension::fromPercentage(100.0f)) {
    Font* ubuntuFont28 = FontManager::getInstance()->loadAndFetchFont("../fonts/Ubuntu-M.ttf", 28);


    auto smallerMenu = new PositioningLayout(Dimension::fromPercentage(70.0f), Dimension::fromPercentage(70.0f));

    auto graphic = new HUDGraphic(ResourceManager::loadAndFetchTexture("../assets/meshes/alphablue.png").get());
    graphic->setRoundedCorners(5);
    graphic->setBorder(5, HUDGraphic::Color(50,50,50));
    smallerMenu->setBackground(graphic);

    TextLayout *youWinText = new TextLayout("You win!", ubuntuFont28, Dimension::fromPixels(200), Dimension::fromPixels(32));
    smallerMenu->addChild(youWinText, Dimension::fromPercentagePlusPixels(50.0f, -100), Dimension::fromPercentage(30.0f));

    TextLayout *timeText = new TextLayout("Time: " + std::to_string(elapsedTimeSeconds), ubuntuFont28, Dimension::fromPixels(200), Dimension::fromPixels(32));
    smallerMenu->addChild(timeText, Dimension::fromPercentagePlusPixels(50.0f, -100), Dimension::fromPercentage(50.0f));

    addChild(smallerMenu, Dimension::fromPercentage(15), Dimension::fromPercentage(15));


}