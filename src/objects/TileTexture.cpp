//
// Created by simon on 2019-04-27.
//

#include <memory>
#include <vector>
#include <Logger.h>
#include <glutil/glutil.h>
#include "TileTexture.h"

TileTexture::TileTexture(std::shared_ptr<std::vector<std::vector<char>>> tiles) : Texture(){
    this->tiles = tiles;
}

void TileTexture::loadTexture() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint texid;
    glGenTextures(1, &texid);
    glActiveTexture(GL_TEXTURE0);
    CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, texid);
    CHECK_GL_ERROR();

    height = tiles->size();
    width = tiles->at(0).size();
    GLubyte image[height][width][4];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char &tile = tiles->at(y).at(x);
            if(tile == '#' || (tile > 'A' && tile < 'Z' && tile != 'V' && tile != 'H')) {
                image[height - 1 - y][x][0] = 128;
                image[height - 1 - y][x][1] = 128;
                image[height - 1 - y][x][2] = 128;
                image[height - 1 - y][x][3] = 255;
            } else {
                image[height - 1 - y][x][0] = 0;
                image[height - 1 - y][x][1] = 200;
                image[height - 1 - y][x][2] = 0;
                image[height - 1 - y][x][3] = 255;
            }
        }
    }


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    CHECK_GL_ERROR();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    CHECK_GL_ERROR();
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    CHECK_GL_ERROR();
    textureID = texid;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    Logger::logInfo("Loaded tile texture.");
}

