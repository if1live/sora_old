// Ŭnicode please

#include "yukino_stdafx.h"

#include "book.h"
#include "book_paper.h"
#include "book_paper_builder.h"
#include "glassless3d.h"
#include "book_scene.h"
#include "sora/xml_reader.h"
#include "sora/xml_node.h"

#include "sora/memory_file.h"
#include "sora/common_string.h"
#include "sora/texture.h"
#include "sora/filesystem.h"
#include "sora/texture_atlas.h"

using namespace std;
using namespace sora;

namespace yukino {;

sora::Texture tex;
TextureAtlas tex_atlas;

////////////////
BookScene::BookScene()
  : useGrid_(true) {
}
BookScene::~BookScene() {
}
TextureSubImage *BookScene::getSprite(const std::string &name) {
  SpriteDictType::iterator it = spriteDict_.find(name);
  if(it == spriteDict_.end()) {
    return NULL;
  } else {
    return &(it->second);
  }
}
void BookScene::parseSpriteNode(sora::XmlNode *node) {
  const string &name = node->GetAttribute("name");
  const string &res = node->GetAttribute("res");
  const string &xStr = node->GetAttribute("x");
  const string &yStr = node->GetAttribute("y");
  const string &wStr = node->GetAttribute("w");
  const string &hStr = node->GetAttribute("h");

  SR_ASSERT(name.size() > 0);
  SR_ASSERT(res.size() > 0);

  //텍스쳐 불러오기. TODO: 텍스쳐를 쌩으로 매번 부르지말고, 일단 생성된 텍스쳐 목록에 존재하는지 확인하고 없으면 부르자
  //@XXX
  /*
  bool texExist = TextureManager::GetInstance().has(res);
  TexturePtr texPtr;
  if(texExist == true) {
    texPtr = TextureManager::GetInstance().get(res);
  } else {
    string resPath = Path::appPath(res);
    TextureDescPtr texDesc = ImageLoader::loader().loadPng2Tex(resPath);
    texPtr = TextureManager::GetInstance().Create(texDesc, res);
  }
  */
  if(tex.handle == 0) {
    string resPath = sora::Filesystem::GetAppPath(res);
    sora::Texture::LoadFromPNG(resPath, &tex);
  }


  //텍스쳐의 정보를 사용해서 x, y, w, h를 구성하기
  float x = 0;
  float y = 0;
  float w = 0;
  float h = 0;
  if(xStr.size() > 0) {
    x = StringToInt(xStr);
    SR_ASSERT(x >= 0);
  }
  if(yStr.size() > 0) {
    y = StringToInt(yStr);
    SR_ASSERT(y >= 0);
  }
  if(wStr.size() > 0) {
    w = StringToInt(wStr);
  } else {
    w = tex.tex_header.src_width;
  }
  if(hStr.size() > 0) {
    h = StringToInt(hStr);
  } else {
    h = tex.tex_header.src_height;
  }
  SR_ASSERT(w > 0);
  SR_ASSERT(h > 0);

  //텍스쳐+크기로 atlas 구성하기. 텍스쳐 아틀라스에 등록시의 이름은
  //res자체의 이름으로 쓰자
  tex_atlas.tex = &tex; ///@FIXME texture 대충 연결
  TextureSubImage &segment = tex_atlas.AddSubImage(name, x, y, w, h);
  spriteDict_[name] = segment;
}
void BookScene::parseSpriteListNode(sora::XmlNode *node) {
  XmlNodeListConstIterator it = node->ChildBegin();
  XmlNodeListConstIterator endit = node->ChildEnd();
  for( ; it != endit ; it++) {
    XmlNode *spriteNode = *it;
    parseSpriteNode(spriteNode);
  }
}
void BookScene::Add(const BookPaper &paper) {
  paperList_.push_back(paper);
}
void BookScene::draw() {
  //등록된 장면을 적절히 그리기 
  BOOST_FOREACH(BookPaper &paper, paperList_) {
    paper.draw();
  }
}
void BookScene::sortPaper() {
  //배경과 관련된것은 무조건 먼저 그린다
  //남은것은 z를 기준으로 정렬
  //오름차순으로 정렬하기
  std::sort(paperList_.begin(), paperList_.end(), BookPaperCompare);
}

void BookScene::parsePaperNode(sora::XmlNode *node) {
  SR_ASSERT(node->name() == "paper");
  const string &typeStr = node->GetAttribute("type");
  SR_ASSERT(typeStr.length() > 0);
  BookPaperType paperType = typeStr2type(typeStr);

  if(paperType == kBookPaperNormal) {
    parseNormalPaperNode(node);
  } else {
    parseWallPaperNode(node);
  }
}
void BookScene::parseNormalPaperNode(sora::XmlNode *node) {
  float cube_width = Book::GetInstance().width;
  float cube_height = Book::GetInstance().height;
  float cube_depth = Book::GetInstance().depth;
  BookPaperBuilder factory(cube_width, cube_height, cube_depth);

  float x = 0;
  float y = 0;
  float z = 0;
  float w = 1;
  float h = 0;
  float yaw = 0;
  float pitch = 0;
  float roll = 0;

  const int useYaw = 0x01;
  const int usePitch = 0x02;
  const int useRoll = 0x04;
  int rotateFlag = 0;

  XmlNodeListConstIterator it = node->ChildBegin();
  XmlNodeListConstIterator endit = node->ChildEnd();
  for( ; it != endit ; it++) {
    XmlNode *attr = *it;
    if(attr->name() == "pos") {
      //pos node
      const string &xStr = attr->GetAttribute("x");
      const string &yStr = attr->GetAttribute("y");
      const string &zStr = attr->GetAttribute("z");
      SR_ASSERT(xStr.length() > 0);
      SR_ASSERT(yStr.length() > 0);
      SR_ASSERT(zStr.length() > 0);

      x = atof(xStr.c_str());
      y = atof(yStr.c_str());
      z = atof(zStr.c_str());
    } else if(attr->name() == "size") {
      //size node
      const string &wStr = attr->GetAttribute("w");
      const string &hStr = attr->GetAttribute("h");
      SR_ASSERT(wStr.length() > 0);
      SR_ASSERT(hStr.length() > 0);

      w = atof(wStr.c_str());
      h = atof(hStr.c_str());
    }  else if(attr->name() == "rotate") {
      //rotate node
      const string &yawStr = attr->GetAttribute("yaw");
      const string &rollStr = attr->GetAttribute("roll");
      const string &pitchStr = attr->GetAttribute("pitch");

      if(yawStr.length() > 0) {
        yaw = atof(yawStr.c_str());
        rotateFlag = useYaw;
      } else if(rollStr.length() > 0) {
        roll = atof(rollStr.c_str());
        rotateFlag = useRoll;
      } else if(pitchStr.length() > 0) {
        pitch = atof(pitchStr.c_str());
        rotateFlag = usePitch;
      } else {
        SR_ASSERT(!"not valid rotation");
      }
    }
  }

  //sprite 얻기
  const string &spriteName = node->GetAttribute("sprite");
  SR_ASSERT(spriteName.size() > 0);
  TextureSubImage &sprite = spriteDict_[spriteName];

  //Create
  if(rotateFlag == 0) {
    paperList_.push_back(factory.CreateNormal(&sprite, x, y, z, w, h));
  } else if(rotateFlag == useYaw) {
    paperList_.push_back(factory.CreateNormalWithYaw(&sprite, x, y, z, w, h, yaw));
  } else if(rotateFlag == useRoll) {
    paperList_.push_back(factory.CreateNormalWithRoll(&sprite, x, y, z, w, h, roll));
  } else if(rotateFlag == usePitch) {
    paperList_.push_back(factory.CreateNormalWithPitch(&sprite, x, y, z, w, h, pitch));
  } else {
    SR_ASSERT(!"not valid");
  }
}
void BookScene::parseWallPaperNode(sora::XmlNode *node) {
  const string &typeStr = node->GetAttribute("type");
  BookPaperType type = typeStr2type(typeStr);

  float cube_width = Book::GetInstance().width;
  float cube_height = Book::GetInstance().height;
  float cube_depth = Book::GetInstance().depth;
  BookPaperBuilder factory(cube_width, cube_height, cube_depth);

  //sprite 얻기
  const string &spriteName = node->GetAttribute("sprite");
  SR_ASSERT(spriteName.size() > 0);
  TextureSubImage *sprite = &(spriteDict_[spriteName]);

  switch(type) {
  case kBookPaperLeft:
    paperList_.push_back(factory.CreateLeft(sprite));
    break;
  case kBookPaperRight:
    paperList_.push_back(factory.CreateRight(sprite));
    break;
  case kBookPaperFloor:
    paperList_.push_back(factory.CreateFloor(sprite));
    break;
  case kBookPaperCeil:
    paperList_.push_back(factory.CreateCeil(sprite));
    break;
  case kBookPaperForward:
    paperList_.push_back(factory.CreateForward(sprite));
    break;
  default:
    SR_ASSERT(!"not valid paper type");
  }
}
BookPaperType BookScene::typeStr2type(const std::string &str) {
  if(str == "left") {
    return kBookPaperLeft;
  } else if(str == "right") {
    return kBookPaperRight;
  } else if(str == "floor") {
    return kBookPaperFloor;
  } else if(str == "ceil") {
    return kBookPaperCeil;
  } else if(str == "forward") {
    return kBookPaperForward;
  } else if(str == "normal") {
    return kBookPaperNormal;
  } else {
    SR_ASSERT(!"not support yet");
  }
}
bool BookScene::isUseGrid() const {
  return useGrid_;
}
void BookScene::setUseGrid(bool b) {
  useGrid_ = b;
}
void BookScene::parsePaperListNode(sora::XmlNode *node) {
  XmlNodeListIterator it = node->ChildBegin();
  XmlNodeListIterator endit = node->ChildEnd();
  for( ; it != endit ; it++) {
    XmlNode *paper = *it;
    parsePaperNode(paper);
  }
}
void BookScene::parseSceneNode(sora::XmlNode *node) {
  //grid 정보 얻기
  const string &gridStr = node->GetAttribute("grid");
  if(gridStr.size() == 0 || gridStr == "0") {
    useGrid_ = false;
  } else {
    useGrid_ = true;
  }

  XmlNodeListIterator it = node->ChildBegin();
  XmlNodeListIterator endit = node->ChildEnd();
  for( ; it != endit ; it++) {
    XmlNode *child = *it;
    if(child->name() == "sprite_list") {
      parseSpriteListNode(child);
    } else if(child->name() == "paper_list") {
      parsePaperListNode(child);
    } else {
      SR_ASSERT(!"not vaild xml");
    }
  }
}

void BookScene::load(const std::string &path) {
  sora::MemoryFile file(path);
  file.Open();
  string content((char*)file.start);
  XmlReader xmlreader;
  XmlNode root;
  bool read_result = xmlreader.Read(&root, content);
  if(!read_result) {
    LOGI("Book scene xml syntax error");
    LOGI(xmlreader.GetError()->str().c_str());
    getchar();
    exit(-1);
  } else {
    LOGI("%s load complete", path.c_str());
  }

  //불러오기전에 기본속성 비우기
  spriteDict_.clear();
  //texAtlasMgr_.clear();
  paperList_.clear();
  useGrid_ = true;
  parseSceneNode(&root);

  //불러온 종이를 정렬하기
  sortPaper();
}

bool BookPaperCompare(const BookPaper &a, const BookPaper &b) {
  BookPaperType a_type = a.type();
  BookPaperType b_type = b.type();
  if(a_type == kBookPaperNormal && b_type == kBookPaperNormal) {
    //둘다 normal
    //z축 비교return (a->getZ() < b->getZ());
    return (a.pos_z < b.pos_z);
  } else if(a_type == kBookPaperNormal && b_type != kBookPaperNormal) {
    //a만 normal
    return (a.pos_z < -100000000);
  } else if(a_type != kBookPaperNormal && b_type == kBookPaperNormal) {
    //b만 normal
    return (-10000000< b.pos_z);
  } else {
    //우선순위는 enum에 있는것을 따라가자
    return a_type < b_type;
  }
}
}