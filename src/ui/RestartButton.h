#pragma once
#include <TextLayout.h>
#include <HUDGraphic.h>

class RestartButton : public TextLayout {
public:
    RestartButton(std::function<void()> startCallback);
};



