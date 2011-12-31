// Ŭnicode please
#pragma once

#include "book_paper.h"
#include "sora/xml_node.h"

namespace yukino {;
class BookPaper;
class BookScene;
class BookPaperFactory;
class BookPaperBuilder;

typedef std::tr1::shared_ptr<BookScene> BookScenePtr;
class BookScene {
public:
  typedef std::tr1::unordered_map<std::string, TextureAtlasSegment*> SpriteDictType;

public:
  BookScene();
  ~BookScene();
  void add(BookPaperPtr paper);
  void sortPaper();
  void draw();

  bool isUseGrid() const;
  void setUseGrid(bool b);

  //장면에 필요한 스프라이트 목록을 로딩해서 여기에서 관리한다
  void load(const std::string &path);

  void parseSpriteNode(sora::XmlNodePtr node);
  void parseSpriteListNode(sora::XmlNodePtr node);
  TextureAtlasSegment *getSprite(const std::string &name) const;

  void parseSceneNode(sora::XmlNodePtr node);
  void parsePaperNode(sora::XmlNodePtr node);
  void parsePaperListNode(sora::XmlNodePtr node);
  void parseNormalPaperNode(sora::XmlNodePtr node);
  void parseWallPaperNode(sora::XmlNodePtr node);

  BookPaperType typeStr2type(const std::string &str);
private:
  bool useGrid_;
  std::vector<BookPaperPtr> paperList_;

  SpriteDictType spriteDict_;
  
  //텍스쳐 아틀라스 관리용
  //TextureAtlasManager texAtlasMgr_;
};
bool BookPaperPtrCompare(BookPaperPtr a, BookPaperPtr b);
}