#ifndef SETTINGS_H_
#define SETTINGS_H_

#include "player.h"
#include "map.h"

namespace rcg {

struct Settings final {
  Player player;
  Map map;
};

} // namespace rcg

#endif // SETTINGS_H_