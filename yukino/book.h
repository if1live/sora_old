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

  float getWidth() const { return width_; }
  float getHeight() const { return height_; }
  float getDepth() const { return depth_; }

  void setWidth(float w) { width_ = w; }
  void setHeight(float h) { height_ = h; }
  void setDepth(float d) { depth_ = d; }

  float getMaxPanDegree() const { return maxPanDegree_; }
  float getMaxTiltDegree() const { return maxTiltDegree_; }
  float getCamRadius() const { return camRadius_; }

  void load(const std::string &cfgfile);
  float calcPanDegree(float rawDegree);
  float calcTiltDegree(float rawDegree);

  const std::vector<std::string> &getSceneFileList() const;
  int getSceneCount() const;
  const std::string &getSceneFile(int index) const;
  const std::string &getCurrSceneFile() const;

  int getCurrScenePage() const;
  void moveScene(int index);
  void moveNextScene();
  void movePrevScene();
  bool isNextSceneExist() const;
  bool isPrevScenExist() const;
private:
  //xml node별 파싱코드
  void loadConfigList(sora::XmlNode *node);
  void loadSceneList(sora::XmlNode *node);

  float width_;
  float height_;
  float depth_;

  float maxPanDegree_;
  float maxTiltDegree_;
  float camRadius_;

  std::vector<std::string> sceneFileList_;

  int currScenePage_;	//현재 몇번쨰 페이를 보는중인가
};
}