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
  void Init();
  void InitGrid();

  void Draw();

  void Update(float dt);
  

  Glassless3d();
  ~Glassless3d();

  void set_visible(bool b) { visible_ = b; }

  void ReloadBook();

  //event
  void OnSceneChangeOccur();

private:
  //sora::model::SoraModelPtr gridModel_;
  //sora::model::SoraModelPtr targetModel_;
  //std::auto_ptr<InputHandler> handler_;

  //unsigned int texId_;
  BookScenePtr scene_;

  bool visible_;
};
}