//
// Created by simon on 2019-04-27.
//

#ifndef HALLSOFBUBBA_MENU_H
#define HALLSOFBUBBA_MENU_H

#include "PositioningLayout.h"


class Menu: public PositioningLayout {
public:
    Menu(Texture* background, std::function<void()> startCallback);

};


#endif //HALLSOFBUBBA_MENU_H
