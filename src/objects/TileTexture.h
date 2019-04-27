//
// Created by simon on 2019-04-27.
//

#ifndef HALLSOFBUBBA_TILETEXTURE_H
#define HALLSOFBUBBA_TILETEXTURE_H

#include <Texture.h>

class TileTexture: public Texture {
public:
    TileTexture(std::shared_ptr<std::vector<std::vector<char>>> tiles);
    void loadTexture() override;

private:
    std::shared_ptr<std::vector<std::vector<char>>> tiles;

};


#endif //HALLSOFBUBBA_TILETEXTURE_H
