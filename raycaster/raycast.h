#ifndef RAYCAST_H_
#define RAYCAST_H_

#include <SDL2/SDL.h>

#include <vector>

#include "adapt/SDL/color.h"
#include "util/map.h"
#include "util/player.h"

namespace rcg {

using ColorMap = std::vector<SDL::Color>;

ColorMap RayCast(const Player &player, const Map &map, int width, int height,
                 float depth = 16.0f);
void drawScreen(SDL_Texture *screen, SDL_Surface *wall_img_surf,
                const Player &player, const Map &map, int height, int width);



}  // namespace rcg

#endif  // RAYCAST_H_