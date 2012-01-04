// Ŭnicode please
#include "yukino_stdafx.h"
#include "book.h"
#include "sora/xml_reader.h"
#include "sora/xml_node.h"
#include "sora/common_string.h"
#include "sora/memory_file.h"
#include "sora/template_library.h"
#include "sora/filesystem.h"
#include "book_scene.h"

using namespace sora;
using namespace std;

namespace yukino {;
Book::Book()
  :	width(480), 
  height(320), 
  depth(320), 
  max_pan_deg_(60),
  max_tilt_deg_(60),
  curr_scene_page_(0),
  cam_radius_(1) {
}
Book::~Book() {
  DestroyList(&scene_list_);
}


float Book::CalcPanDeg(float raw_degree) {
  if(raw_degree > max_pan_deg_) {
    return max_pan_deg_;
  } else if(raw_degree < -max_pan_deg_)   {
    return -max_pan_deg_;
  }
  //else..
  return raw_degree;
}
float Book::CalcTiltDeg(float raw_degree) {
  if(raw_degree > max_tilt_deg_) {
    return max_tilt_deg_;
  } else if(raw_degree < -max_tilt_deg_) {
    return -max_tilt_deg_;
  }
  //else..
  return raw_degree;
}
const std::string &Book::GetSceneFile(int index) const {
  return scene_file_list_[index];
}
const std::string &Book::GetCurrSceneFile() const {
  return scene_file_list_[curr_scene_page_];
}
void Book::LoadConfigList(sora::XmlNode *node) {
  XmlNodeListIterator it = node->ChildBegin();
  XmlNodeListIterator endit = node->ChildEnd();
  for( ; it != endit ; it++) {
    XmlNode *node = *it;
    const std::string &key = node->GetAttribute("key");
    const std::string &value = node->GetAttribute("value");
    if(key == "width") {
      int width = sora::StringToInt(value);
      SR_ASSERT(width > 0);
      this->width = width;
    } else if(key == "height") {
      int height = sora::StringToInt(value);
      SR_ASSERT(height > 0);
      this->height = height;
    } else if(key == "depth") {
      int depth = sora::StringToInt(value);
      SR_ASSERT(depth > 0);
      this->depth = depth;
    } else if(key == "max_pan_degree") {
      float deg = sora::StringToFloat(value);
      SR_ASSERT(deg > 0);
      max_pan_deg_ = deg;
    } else if(key == "max_tilt_degree") {
      float deg = sora::StringToFloat(value);
      SR_ASSERT(deg > 0);
      max_tilt_deg_ = deg;
    } else if(key == "cam_radius") {
      float radius = sora::StringToFloat(value);
      SR_ASSERT(radius > 0);
      cam_radius_ = radius;
    } else {
      SR_ASSERT(!"not valid cube config xml node");
    }
  }
}
void Book::LoadSceneList(sora::XmlNode *node) {
  scene_file_list_.clear();

  XmlNodeListIterator it = node->ChildBegin();
  XmlNodeListIterator endit = node->ChildEnd();
  for( ; it != endit ; it++) {
    XmlNode *sceneNode = *it;
    const string &file = sceneNode->GetAttribute("file");
    SR_ASSERT(file.size() > 0 && "no file error");
    scene_file_list_.push_back(file);
  }

  //scene 파일 전부 로딩하기
  BOOST_FOREACH(const std::string &file, scene_file_list_) {
    string path = sora::Filesystem::GetAppPath(file);
    sora::MemoryFile mem_file(path);
    mem_file.Open();

    const char *content = (const char*)mem_file.start;
    XmlReader xml_reader;
    XmlNode *node = new XmlNode();
    bool read_result = xml_reader.Read(node, content);
    SR_ASSERT(read_result == true);

    BookScene *scene = new BookScene(node);
    scene_list_.push_back(scene);

    mem_file.Close();
  }
}
void Book::Load(const std::string &cfgfile) {
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
  XmlReader reader;
  XmlNode root;
  bool read_result = reader.Read(&root, (char*)file.start);
  if(!read_result) {
    LOGE("Book xml syntax error, ctrl+c is quit");
    LOGE(reader.GetError()->str().c_str());
    getchar();
    exit(-1);
  }
  XmlNodeListIterator it = root.ChildBegin();
  XmlNodeListIterator endit = root.ChildEnd();
  for( ; it != endit ; it++) {
    XmlNode *node = *it;
    if(node->name() == "config_list") {
      LoadConfigList(node);
    } else if(node->name() == "scene_list") {
      LoadSceneList(node);
    } else {
      SR_ASSERT(!"not valid xml");
    }
  }

  file.Close();
}

void Book::MoveNextScene() {
  if(IsNextSceneExist() == true) {
    curr_scene_page_++;
  }
}
void Book::MovePrevScene() {
  if(IsPrevScenExist() == true) {
    curr_scene_page_--;
  }
}
bool Book::IsNextSceneExist() const {
  if(curr_scene_page() >= scene_file_list_.size()-1) {
    return false;
  } else {
    return true;
  }
}
bool Book::IsPrevScenExist() const {
  if(curr_scene_page() == 0) {
    return false;
  } else {
    return true;
  }
}

void Book::MoveScene(int index) {
  SR_ASSERT(index >= 0 && index < scene_file_list_.size());
  curr_scene_page_ = index;
}

}