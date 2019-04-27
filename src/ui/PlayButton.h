//
// Created by simon on 2019-04-27.
//

#ifndef HALLSOFBUBBA_PLAYBUTTON_H
#define HALLSOFBUBBA_PLAYBUTTON_H

#include <TextLayout.h>
#include <HUDGraphic.h>

class PlayButton: public TextLayout {
public:
    PlayButton(std::function<void()> startCallback);

};


#endif //HALLSOFBUBBA_PLAYBUTTON_H
