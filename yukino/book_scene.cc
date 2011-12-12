// Ŭnicode please

#include "yukino_stdafx.h"

#include "book.h"
#include "book_paper.h"
#include "book_paper_builder.h"
#include "glassless3d.h"
#include "book_scene.h"
#include "xml_reader.h"

#include "sora/memory_file.h"
#include "sora/common_string.h"
#include "sora/texture.h"
#include "sora/filesystem.h"
#include "texture_atlas.h"

using namespace std;
using namespace mio;
using namespace sora;

namespace yukino {;

sora::Texture tex;
TextureAtlas tex_atlas;

////////////////
BookScene::BookScene()
  : useGrid_(true)
{

}
BookScene::~BookScene()
{
}
TextureAtlasSegment *BookScene::getSprite(const std::string &name) const
{
  SpriteDictType::const_iterator it = spriteDict_.find(name);
  if(it == spriteDict_.end())
  {
    return NULL;
  }
  else
  {
    return it->second;
  }
}
void BookScene::parseSpriteNode(mio::XmlNodePtr node)
{
  const string &name = node->getAttribute("name");
  const string &res = node->getAttribute("res");
  const string &xStr = node->getAttribute("x");
  const string &yStr = node->getAttribute("y");
  const string &wStr = node->getAttribute("w");
  const string &hStr = node->getAttribute("h");

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
    texPtr = TextureManager::GetInstance().create(texDesc, res);
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
  if(xStr.size() > 0)
  {
    x = StringToInt(xStr);
    SR_ASSERT(x >= 0);
  }
  if(yStr.size() > 0)
  {
    y = StringToInt(yStr);
    SR_ASSERT(y >= 0);
  }
  if(wStr.size() > 0)
  {
    w = StringToInt(wStr);
  }
  else
  {
    w = tex.tex_header.src_width;
  }
  if(hStr.size() > 0)
  {
    h = StringToInt(hStr);
  }
  else
  {
    h = tex.tex_header.src_height;
  }
  SR_ASSERT(w > 0);
  SR_ASSERT(h > 0);

  //텍스쳐+크기로 atlas 구성하기. 텍스쳐 아틀라스에 등록시의 이름은
  //res자체의 이름으로 쓰자
  tex_atlas.tex = &tex; ///@FIXME texture 대충 연결
  TextureAtlasSegment *segment = tex_atlas.AddSegment(name, x, y, w, h);
  spriteDict_[name] = segment;
}
void BookScene::parseSpriteListNode(mio::XmlNodePtr node)
{
  XmlNodePtrIter it = node->childBegin();
  XmlNodePtrIter endit = node->childEnd();
  for( ; it != endit ; it++)
  {
    XmlNodePtr spriteNode = *it;
    parseSpriteNode(spriteNode);
  }
}
void BookScene::add(BookPaperPtr paper)
{
  paperList_.push_back(paper);
}
void BookScene::draw()
{
  //등록된 장면을 적절히 그리기 
  BOOST_FOREACH(BookPaperPtr paper, paperList_)
  {
    paper->draw();
  }
}
void BookScene::sortPaper()
{
  //배경과 관련된것은 무조건 먼저 그린다
  //남은것은 z를 기준으로 정렬
  //오름차순으로 정렬하기
  std::sort(paperList_.begin(), paperList_.end(), BookPaperPtrCompare);
}

void BookScene::parsePaperNode(mio::XmlNodePtr node)
{
  SR_ASSERT(node->getName() == "paper");
  const string &typeStr = node->getAttribute("type");
  SR_ASSERT(typeStr.length() > 0);
  BookPaperType paperType = typeStr2type(typeStr);

  if(paperType == BookPaperTypeNormal)
  {
    parseNormalPaperNode(node);
  }
  else
  {
    parseWallPaperNode(node);
  }
}
void BookScene::parseNormalPaperNode(mio::XmlNodePtr node)
{
  float cubeWidth = Book::GetInstance().getWidth();
  float cubeHeight = Book::GetInstance().getHeight();
  float cubeDepth = Book::GetInstance().getDepth();
  BookPaperFactory factory(cubeWidth, cubeHeight, cubeDepth);

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

  XmlNodePtrIter it = node->childBegin();
  XmlNodePtrIter endit = node->childEnd();
  for( ; it != endit ; it++)
  {
    XmlNodePtr attr = *it;
    if(attr->getName() == "pos")
    {
      //pos node
      const string &xStr = attr->getAttribute("x");
      const string &yStr = attr->getAttribute("y");
      const string &zStr = attr->getAttribute("z");
      SR_ASSERT(xStr.length() > 0);
      SR_ASSERT(yStr.length() > 0);
      SR_ASSERT(zStr.length() > 0);

      x = atof(xStr.c_str());
      y = atof(yStr.c_str());
      z = atof(zStr.c_str());
    }
    else if(attr->getName() == "size")
    {
      //size node
      const string &wStr = attr->getAttribute("w");
      const string &hStr = attr->getAttribute("h");
      SR_ASSERT(wStr.length() > 0);
      SR_ASSERT(hStr.length() > 0);

      w = atof(wStr.c_str());
      h = atof(hStr.c_str());
    }
    else if(attr->getName() == "rotate")
    {
      //rotate node
      const string &yawStr = attr->getAttribute("yaw");
      const string &rollStr = attr->getAttribute("roll");
      const string &pitchStr = attr->getAttribute("pitch");

      if(yawStr.length() > 0)
      {
        yaw = atof(yawStr.c_str());
        rotateFlag = useYaw;
      }
      else if(rollStr.length() > 0)
      {
        roll = atof(rollStr.c_str());
        rotateFlag = useRoll;
      }
      else if(pitchStr.length() > 0)
      {
        pitch = atof(pitchStr.c_str());
        rotateFlag = usePitch;
      }
      else
      {
        SR_ASSERT(!"not valid rotation");
      }
    }
  }

  //sprite 얻기
  const string &spriteName = node->getAttribute("sprite");
  SR_ASSERT(spriteName.size() > 0);
  TextureAtlasSegment *sprite = spriteDict_[spriteName];

  //create
  BookPaperPtr paper;
  if(rotateFlag == 0)
  {
    paper = factory.createNormal(sprite, x, y, z, w, h);
  }
  else if(rotateFlag == useYaw)
  {
    paper = factory.createNormalWithYaw(sprite, x, y, z, w, h, yaw);
  }
  else if(rotateFlag == useRoll)
  {
    paper = factory.createNormalWithRoll(sprite, x, y, z, w, h, roll);
  }
  else if(rotateFlag == usePitch)
  {
    paper = factory.createNormalWithPitch(sprite, x, y, z, w, h, pitch);
  }
  else
  {
    SR_ASSERT(!"not valid");
  }

  paperList_.push_back(paper);
}
void BookScene::parseWallPaperNode(mio::XmlNodePtr node)
{
  const string &typeStr = node->getAttribute("type");
  BookPaperType type = typeStr2type(typeStr);

  float cubeWidth = Book::GetInstance().getWidth();
  float cubeHeight = Book::GetInstance().getHeight();
  float cubeDepth = Book::GetInstance().getDepth();
  BookPaperFactory factory(cubeWidth, cubeHeight, cubeDepth);

  //sprite 얻기
  const string &spriteName = node->getAttribute("sprite");
  SR_ASSERT(spriteName.size() > 0);
  TextureAtlasSegment *sprite = spriteDict_[spriteName];

  BookPaperPtr paper;
  switch(type) {
  case BookPaperTypeLeft:
    paper = factory.createLeft(sprite);
    break;
  case BookPaperTypeRight:
    paper = factory.createRight(sprite);
    break;
  case BookPaperTypeFloor:
    paper = factory.createFloor(sprite);
    break;
  case BookPaperTypeCeil:
    paper = factory.createCeil(sprite);
    break;
  case BookPaperTypeForward:
    paper = factory.createForward(sprite);
    break;
  default:
    SR_ASSERT(!"not valid paper type");
  }
  paperList_.push_back(paper);
}
BookPaperType BookScene::typeStr2type(const std::string &str)
{
  if(str == "left")
  {
    return BookPaperTypeLeft;
  }
  else if(str == "right")
  {
    return BookPaperTypeRight;
  }
  else if(str == "floor")
  {
    return BookPaperTypeFloor;
  }
  else if(str == "ceil")
  {
    return BookPaperTypeCeil;
  }
  else if(str == "forward")
  {
    return BookPaperTypeForward;
  }
  else if(str == "normal")
  {
    return BookPaperTypeNormal;
  }
  else
  {
    SR_ASSERT(!"not support yet");
  }
}
bool BookScene::isUseGrid() const
{
  return useGrid_;
}
void BookScene::setUseGrid(bool b)
{
  useGrid_ = b;
}
void BookScene::parsePaperListNode(mio::XmlNodePtr node)
{
  XmlNodePtrIter it = node->childBegin();
  XmlNodePtrIter endit = node->childEnd();
  for( ; it != endit ; it++)
  {
    XmlNodePtr paper = *it;
    parsePaperNode(paper);
  }
}
void BookScene::parseSceneNode(mio::XmlNodePtr node)
{
  //grid 정보 얻기
  const string &gridStr = node->getAttribute("grid");
  if(gridStr.size() == 0 || gridStr == "0")
  {
    useGrid_ = false;
  }
  else
  {
    useGrid_ = true;
  }

  XmlNodePtrIter it = node->childBegin();
  XmlNodePtrIter endit = node->childEnd();
  for( ; it != endit ; it++)
  {
    XmlNodePtr child = *it;
    if(child->getName() == "sprite_list")
    {
      parseSpriteListNode(child);
    }
    else if(child->getName() == "paper_list")
    {
      parsePaperListNode(child);
    }
    else
    {
      SR_ASSERT(!"not vaild xml");
    }
  }
}

void BookScene::load(const std::string &path)
{
  sora::MemoryFile file(path);
  file.Open();
  string content((char*)file.start);
  XmlReader xmlreader;
  XmlNodePtr root = xmlreader.read(content);
  if(root->isNull()) {
    LOGI("Book scene xml syntax error");
    LOGI(xmlreader.getError()->str().c_str());
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
  parseSceneNode(root);

  //불러온 종이를 정렬하기
  sortPaper();
}

bool BookPaperPtrCompare(BookPaperPtr a, BookPaperPtr b)
{
  BookPaperType aType = a->getType();
  BookPaperType bType = b->getType();
  if(aType == BookPaperTypeNormal && bType == BookPaperTypeNormal)
  {
    //둘다 normal
    //z축 비교return (a->getZ() < b->getZ());
    return (a->getZ() < b->getZ());
  }
  else if(aType == BookPaperTypeNormal && bType != BookPaperTypeNormal)
  {
    //a만 normal
    return (a->getZ() < -100000000);
  }
  else if(aType != BookPaperTypeNormal && bType == BookPaperTypeNormal)
  {
    //b만 normal
    return (-10000000< b->getZ());
  }
  else
  {
    //우선순위는 enum에 있는것을 따라가자
    return aType < bType;
  }
}
}