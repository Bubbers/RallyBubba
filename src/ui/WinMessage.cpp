#include <Dimension.h>
#include <TextLayout.h>
#include <FontManager.h>
#include "WinMessage.h"

WinMessage::WinMessage(): PositioningLayout(Dimension::fromPercentage(100.0f), Dimension::fromPercentage(100.0f)) {
    Font* ubuntuFont28 = FontManager::getInstance()->loadAndFetchFont("../fonts/Ubuntu-M.ttf", 28);

    TextLayout *titleLayout = new TextLayout("You win!", ubuntuFont28, Dimension::fromPixels(200), Dimension::fromPixels(32));
    addChild(titleLayout, Dimension::fromPercentagePlusPixels(50, -100), Dimension::fromPercentage(10));
}