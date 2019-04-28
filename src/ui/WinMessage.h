

#pragma once


#include <PositioningLayout.h>

class WinMessage: public PositioningLayout {
public:
    WinMessage(float elapsedTimeSeconds, std::function<void ()> restartFunc);
private:
};



