// Ŭnicode please
#pragma once

#include "book_paper.h"

namespace sora {;
class XmlNode;
class TextureSubImage;
}

namespace yukino {;
class BookPaper;
class BookScene;
class BookPaperBuilder;

///@TODO book scene 의 메모리 관리를 더 단순화 하기
typedef std::tr1::shared_ptr<BookScene> BookScenePtr;
class BookScene {
public:
  typedef std::tr1::unordered_map<std::string, sora::TextureSubImage> SpriteDictType;

public:
  BookScene();
  ~BookScene();
  void Add(const BookPaper &paper);
  void sortPaper();
  void draw();

  bool isUseGrid() const;
  void setUseGrid(bool b);

  //장면에 필요한 스프라이트 목록을 로딩해서 여기에서 관리한다
  void load(const std::string &path);

  void parseSpriteNode(sora::XmlNode *node);
  void parseSpriteListNode(sora::XmlNode *node);
  sora::TextureSubImage *getSprite(const std::string &name);

  void parseSceneNode(sora::XmlNode *node);
  void parsePaperNode(sora::XmlNode *node);
  void parsePaperListNode(sora::XmlNode *node);
  void parseNormalPaperNode(sora::XmlNode *node);
  void parseWallPaperNode(sora::XmlNode *node);

  BookPaperType typeStr2type(const std::string &str);
private:
  bool useGrid_;
  std::vector<BookPaper> paperList_;

  SpriteDictType spriteDict_;
  
  //텍스쳐 아틀라스 관리용
  //TextureAtlasManager texAtlasMgr_;
};

bool BookPaperCompare(const BookPaper &a, const BookPaper &b);
}