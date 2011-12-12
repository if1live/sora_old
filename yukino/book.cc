// Ŭnicode please
#include "yukino_stdafx.h"
#include "book.h"
#include "xml_reader.h"
#include "xml_node.h"
#include "sora/common_string.h"

#include "sora/memory_file.h"

using namespace mio;
using namespace std;

namespace yukino {;
Book::Book()
  :	width_(480), 
  height_(320), 
  depth_(320), 
  maxPanDegree_(60),
  maxTiltDegree_(60),
  currScenePage_(0),
  camRadius_(1) {
}
Book::~Book() {
}


float Book::calcPanDegree(float rawDegree) {
  if(rawDegree > maxPanDegree_) {
    return maxPanDegree_;
  } else if(rawDegree < -maxPanDegree_)   {
    return -maxPanDegree_;
  }
  //else..
  return rawDegree;
}
float Book::calcTiltDegree(float rawDegree) {
  if(rawDegree > maxTiltDegree_) {
    return maxTiltDegree_;
  } else if(rawDegree < -maxTiltDegree_) {
    return -maxTiltDegree_;
  }
  //else..
  return rawDegree;
}
const std::vector<std::string> &Book::getSceneFileList() const {
  return sceneFileList_;
}
int Book::getSceneCount() const {
  return sceneFileList_.size();
}
const std::string &Book::getSceneFile(int index) const {
  return sceneFileList_[index];
}
const std::string &Book::getCurrSceneFile() const {
  return sceneFileList_[currScenePage_];
}
void Book::loadConfigList(mio::XmlNodePtr node) {
  XmlNodePtrIter it = node->childBegin();
  XmlNodePtrIter endit = node->childEnd();
  for( ; it != endit ; it++) {
    XmlNodePtr node = *it;
    const std::string &key = node->getAttribute("key");
    const std::string &value = node->getAttribute("value");
    if(key == "width") {
      int width = sora::StringToInt(value);
      SR_ASSERT(width > 0);
      setWidth(width);
    } else if(key == "height") {
      int height = sora::StringToInt(value);
      SR_ASSERT(height > 0);
      setHeight(height);
    } else if(key == "depth") {
      int depth = sora::StringToInt(value);
      SR_ASSERT(depth > 0);
      setDepth(depth);
    } else if(key == "max_pan_degree") {
      float deg = sora::StringToFloat(value);
      SR_ASSERT(deg > 0);
      maxPanDegree_ = deg;
    } else if(key == "max_tilt_degree") {
      float deg = sora::StringToFloat(value);
      SR_ASSERT(deg > 0);
      maxTiltDegree_ = deg;
    } else if(key == "cam_radius") {
      float radius = sora::StringToFloat(value);
      SR_ASSERT(radius > 0);
      camRadius_ = radius;
    } else {
      SR_ASSERT(!"not valid cube config xml node");
    }
  }
}
void Book::loadSceneList(mio::XmlNodePtr node) {
  sceneFileList_.clear();

  XmlNodePtrIter it = node->childBegin();
  XmlNodePtrIter endit = node->childEnd();
  for( ; it != endit ; it++) {
    XmlNodePtr sceneNode = *it;
    const string &file = sceneNode->getAttribute("file");
    SR_ASSERT(file.size() > 0 && "no file error");
    sceneFileList_.push_back(file);
  }
}
void Book::load(const std::string &cfgfile) {
  /*
  <?xml version="1.0"?>
  <cube>
  <config key="width" value="1000"/>
  <config key="height" value="1000"/>
  <config key="depth" value="500"/>
  <!-- 중심을 기준으로 좌우로 카메라 움직일수 있는 최대 각도-->
  <config key="max_pan_degree" value="60"/>
  <!-- 중심을 기준으로 상하로 카메라 움직일수 있는 최대 각도-->
  <config key="max_tilt_degree" value="40"/>
  </cube>
  */
  sora::MemoryFile file(cfgfile);
  file.Open();
  char *buffer = (char*)file.start;
  string content(buffer);
  XmlReader reader;
  XmlNodePtr root = reader.read(content);
  if(root->isNull()) {
    LOGE("Book xml syntax error, ctrl+c is quit");
    LOGE(reader.getError()->str().c_str());
    getchar();
    exit(-1);
  }
  XmlNodePtrIter it = root->childBegin();
  XmlNodePtrIter endit = root->childEnd();
  for( ; it != endit ; it++) {
    XmlNodePtr node = *it;
    if(node->getName() == "config_list") {
      loadConfigList(node);
    } else if(node->getName() == "scene_list") {
      loadSceneList(node);
    } else {
      SR_ASSERT(!"not valid xml");
    }
  }

  file.Close();
}

int Book::getCurrScenePage() const {
  return currScenePage_;
}
void Book::moveNextScene() {
  if(isNextSceneExist() == true) {
    currScenePage_++;
  }
}
void Book::movePrevScene() {
  if(isPrevScenExist() == true) {
    currScenePage_--;
  }
}
bool Book::isNextSceneExist() const {
  if(getCurrScenePage() >= sceneFileList_.size()-1) {
    return false;
  } else {
    return true;
  }
}
bool Book::isPrevScenExist() const {
  if(getCurrScenePage() == 0) {
    return false;
  } else {
    return true;
  }
}

void Book::moveScene(int index) {
  SR_ASSERT(index >= 0 && index < sceneFileList_.size());
  currScenePage_ = index;
}

}