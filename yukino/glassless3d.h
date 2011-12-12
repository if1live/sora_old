// Ŭnicode please
#pragma once
#include "sora/template_library.h"
#include "book_scene.h"

//#define CUBE_SCALE 1.115f

namespace sora {
  class Texture;
}

namespace yukino {;
class InputHandler;
class PopupSprite;
class BookScene;

class Glassless3d : public sora::Singleton<Glassless3d> {
public:
  void init();
  void initGrid();

  void draw();

  void update(float dt);
  void onSceneChangeOccur();

  Glassless3d();
  ~Glassless3d();

  void SetVisible(bool b);

  void reloadBook();
private:
  //sora::model::SoraModelPtr gridModel_;
  //sora::model::SoraModelPtr targetModel_;
  //std::auto_ptr<InputHandler> handler_;

  //unsigned int texId_;
  BookScenePtr scene_;

  bool visible_;
};
}