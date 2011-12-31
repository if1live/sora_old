// Ŭnicode please
#pragma once

#include "sora/template_library.h"

namespace sora {;
class XmlNode;
}

namespace yukino {;
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

  const std::vector<std::string> &GetSceneFileList() const { return scene_file_list_; }
  int GetSceneCount() const { return scene_file_list_.size(); }
  const std::string &GetSceneFile(int index) const;
  const std::string &GetCurrSceneFile() const;

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
  //xml node별 파싱코드
  void LoadConfigList(sora::XmlNode *node);
  void LoadSceneList(sora::XmlNode *node);
  

  float max_pan_deg_;
  float max_tilt_deg_;
  float cam_radius_;

  std::vector<std::string> scene_file_list_;

  int curr_scene_page_;	//현재 몇번쨰 페이를 보는중인가
};
}