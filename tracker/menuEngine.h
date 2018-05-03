#include <vector>
#include <SDL.h>
#include "menu.h"

class MenuEngine {
public:
  MenuEngine ();
  ~MenuEngine ();
  int play();

private:
  SDL_Renderer * const renderer;
  Menu menu;

  void draw() const;
  void update(Uint32);

  MenuEngine(const MenuEngine&);
  MenuEngine& operator=(const MenuEngine&);
};
