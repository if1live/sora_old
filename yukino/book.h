// Ŭnicode please
#pragma once

#include "sora/template_library.h"

namespace sora {;
class XmlNode;
}

namespace yukino {;
class BookScene;
class Book : public sora::Singleton<Book> {
public:
  Book();
  ~Book();

  float max_pan_deg() const { return max_pan_deg_; }
  float max_tilt_deg() const { return max_tilt_deg_; }
  float cam_radius() const { return cam_radius_; }

  void Load(const std::string &cfgfile);
  float CalcPanDeg(float raw_degree);
  float CalcTiltDeg(float raw_degree);

  BookScene *GetCurrScene() { return scene_list_[curr_scene_page_]; }
  BookScene *GetScene(int index) { return scene_list_[index]; }

  int curr_scene_page() const { return curr_scene_page_; }
  void MoveScene(int index);
  void MoveNextScene();
  void MovePrevScene();
  bool IsNextSceneExist() const;
  bool IsPrevScenExist() const;

public:
  float width;
  float height;
  float depth;

private:
  //legacy code. xml파일 얼마 되지도 않으니 전부 읽어서 올려놓자
  //올려놓으면 좋은게 필요한씬에 필요할떄 접근할수 있어서, 필요 texture를 미리 load할수있다
  const std::vector<std::string> &GetSceneFileList() const { return scene_file_list_; }
  int GetSceneCount() const { return scene_file_list_.size(); }
  const std::string &GetSceneFile(int index) const;
  const std::string &GetCurrSceneFile() const;

private:
  //xml node별 파싱코드
  void LoadConfigList(sora::XmlNode *node);
  void LoadSceneList(sora::XmlNode *node);
  

  float max_pan_deg_;
  float max_tilt_deg_;
  float cam_radius_;

  std::vector<std::string> scene_file_list_;

  int curr_scene_page_;	//현재 몇번쨰 페이를 보는중인가

  //장면을 구성할 xml을 미리 불러놓는다
  std::vector<BookScene*> scene_list_;
};
}