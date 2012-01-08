// Ŭnicode please
#pragma once

#include "sora/template_library.h"
#include "sora/locale.h"

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

  void Load(const std::string &cfgfile, sora::LanguageType lang);
  float CalcPanDeg(float raw_degree);
  float CalcTiltDeg(float raw_degree);

  BookScene *GetCurrScene();
  BookScene *GetScene(int index);

  int curr_scene_page() const { return curr_scene_page_; }
  void MoveScene(int index);
  void MoveNextScene();
  void MovePrevScene();
  bool IsNextSceneExist() const;
  bool IsPrevScenExist() const;

public:
  void SetLanguage(sora::LanguageType lang);

public:
  float width;
  float height;
  float depth;

private:
  //xml node별 파싱코드
  void LoadConfigList(sora::XmlNode *node, sora::LanguageType lang);
  void LoadSceneList(sora::XmlNode *node, sora::LanguageType lang);
  std::vector<BookScene*> &scene_list();
  const std::vector<BookScene*> &scene_list() const;

  float max_pan_deg_;
  float max_tilt_deg_;
  float cam_radius_;

  int curr_scene_page_;	//현재 몇번쨰 페이를 보는중인가

  //장면을 구성할 xml을 미리 불러놓는다
  std::vector<BookScene*> kor_scene_list_;
  std::vector<BookScene*> eng_scene_list_;
  sora::LanguageType lang_;
};
}