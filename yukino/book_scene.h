// Ŭnicode please
#pragma once

#include "book_paper.h"
#include "sora/render/texture_manager.h"

#if SR_USE_PCH == 0
#include <set>
#endif

namespace sora {;
class XmlNode;
struct TextureSubImage;
}

namespace yukino {;
class BookPaper;
class BookScene;
class BookPaperBuilder;

class BookScene {
public:
  typedef std::tr1::unordered_map<std::string, sora::TextureSubImage> SpriteDictType;

public:
  BookScene();
  BookScene(sora::XmlNode *node);
  ~BookScene();
  void Add(const BookPaper &paper);
  void sortPaper();
  void draw();

  bool isUseGrid() const;
  void setUseGrid(bool b);

  //장면에 필요한 스프라이트 목록을 로딩해서 여기에서 관리한다
  void Load();
  void Unload();
  void LoadTexture();
  void UnloadTexture();

  void parseSpriteNode(sora::XmlNode *node);
  void parseSpriteListNode(sora::XmlNode *node);
  sora::TextureSubImage *getSprite(const std::string &name);

  void parseSceneNode(sora::XmlNode *node);
  void parsePaperNode(sora::XmlNode *node);
  void parsePaperListNode(sora::XmlNode *node);
  void parseNormalPaperNode(sora::XmlNode *node);
  void parseWallPaperNode(sora::XmlNode *node);

  BookPaperType typeStr2type(const std::string &str);

  const std::set<sora::TexturePtr> &tex_handle_list() const { return tex_handle_list_; }
private:
  bool useGrid_;
  std::vector<BookPaper> paperList_;

  SpriteDictType spriteDict_;

  sora::XmlNode *root_node_;

  //해당씬을 표현하는데 사용되는 텍스쳐. 텍스쳐 2장을 쓰는 일단은 없으니까
  std::set<sora::TexturePtr> tex_handle_list_;
};

bool BookPaperCompare(const BookPaper &a, const BookPaper &b);
}