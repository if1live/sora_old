// Ŭnicode please
#include "yukino_stdafx.h"
#include "book_paper.h"
#include "book_paper_builder.h"
#include "book.h"
#include "texture_atlas.h"
#include "sora/texture.h"

using namespace std;
using namespace sora;

namespace yukino {;
//float globalXGap = Book::GetInstance().getWidth() * 0.01;
//float globalYGap = Book::GetInstance().getHeight() * 0.01;
//float globalZGap = Book::GetInstance().getDepth() * 0.01;
float globalXGap = 2;
float globalYGap = 2;
float globalZGap = 4;

BookPaperBuilder::BookPaperBuilder(float cubeWidth, float cubeHeight, float cubeDepth)
  : cubeWidth_(cubeWidth), cubeHeight_(cubeHeight), cubeDepth_(cubeDepth)
{
  SR_ASSERT(cubeWidth_ > 0 && cubeHeight_ > 0 && cubeDepth_ > 0);
  clear();
}

BookPaperBuilder::~BookPaperBuilder()
{
}

void BookPaperBuilder::createFloorModel() {
  //string cubeFile = Path::appPath("res/floor01.jpg");
  //string cubeFile = Path::appPath("res/img_test.bmp");
  //string cubeFile = Path::appPath("res/small_test_2x2.png");
  //string cubeFile = Path::appPath("res/small_test_2x1.png");
  //TextureDescPtr texDesc = ImageLoader::loader().loadJpg2Tex(cubeFile);
  //TexturePtr tex = TextureManager::GetInstance().create(texDesc, string("floor"));

  /*cube model 적당히 생성*/
  /*모델을 생성*/

  //texture atlas를 쓸지도 몰라서 테스트로 짰던 코드이다. 실제로는 결국 안썼다
  //float baseX = 0;
  //float baseY = (float)(tex->getDesc().texHeight - tex->getDesc().origHeight) / tex->getDesc().texHeight;
  //float width = (float)(tex->getDesc().origWidth) / tex->getDesc().texWidth;
  //float height = 1.0 - baseY;
  float texBaseX = sprite_->GetAtlasX();
  float texBaseY = sprite_->GetAtlasY();
  float texWidth = sprite_->GetAtlasW();
  float texHeight = sprite_->GetAtlasH();

  float xGap = globalXGap;
  float zGap = globalZGap;

  //gap=크기*0.01		
  v1.s = texBaseX;
  v1.t = texBaseY+texHeight;
  v1.x = -cubeWidth_/2-xGap;
  v1.y = -cubeHeight_/2;
  v1.z = 0;

  v2.s = texBaseX + texWidth;
  v2.t = texBaseY+texHeight;
  v2.x = cubeWidth_/2+xGap;
  v2.y = -cubeHeight_/2;
  v2.z = 0;

  v3.s = texBaseX + texWidth;
  v3.t = texBaseY;
  v3.x = cubeWidth_/2+xGap;
  v3.y = -cubeHeight_/2;
  v3.z = -cubeDepth_-zGap;		

  v4.s = texBaseX;
  v4.t = texBaseY;
  v4.x = -cubeWidth_/2-xGap;
  v4.y = -cubeHeight_/2;
  v4.z = -cubeDepth_-zGap;
}

void BookPaperBuilder::createLeftModel() 
{
  float texBaseX = sprite_->GetAtlasX();
  float texBaseY = sprite_->GetAtlasY();
  float texWidth = sprite_->GetAtlasW();
  float texHeight = sprite_->GetAtlasH();

  //좌우벽은 gap를 신경쓰지 말자
  float yGap = 0;
  float zGap = 0;

  v1.s = texBaseX;
  v1.t = texBaseY+texHeight;
  v1.vertex3(-cubeWidth_/2, -cubeHeight_/2-yGap, 0);

  v2.texCoord2(texBaseX + texWidth, texBaseY+texHeight);
  v2.vertex3(-cubeWidth_/2, -cubeHeight_/2-yGap, -cubeDepth_-zGap);		

  v3.texCoord2(texBaseX + texWidth, texBaseY);
  v3.vertex3(-cubeWidth_/2, cubeHeight_/2+yGap, -cubeDepth_-zGap);		

  v4.texCoord2(texBaseX, texBaseY);
  v4.vertex3(-cubeWidth_/2, cubeHeight_/2+yGap, 0);
}
void BookPaperBuilder::createRightModel() 
{
  float texBaseX = sprite_->GetAtlasX();
  float texBaseY = sprite_->GetAtlasY();
  float texWidth = sprite_->GetAtlasW();
  float texHeight = sprite_->GetAtlasH();

  //좌우벽은 gap를 신경쓰지 말자
  float yGap = 0;
  float zGap = 0;

  v1.texCoord2(texBaseX, texBaseY+texHeight);
  v1.vertex3(cubeWidth_/2, -cubeHeight_/2-yGap, -cubeDepth_-zGap);

  v2.texCoord2(texBaseX + texWidth, texBaseY+texHeight);
  v2.vertex3(cubeWidth_/2, -cubeHeight_/2-yGap, 0);		

  v3.texCoord2(texBaseX + texWidth, texBaseY);
  v3.vertex3(cubeWidth_/2, cubeHeight_/2+yGap, 0);		

  v4.texCoord2(texBaseX, texBaseY);
  v4.vertex3(cubeWidth_/2, cubeHeight_/2+yGap, -cubeDepth_-zGap);
}
void BookPaperBuilder::createCeilModel() 
{
  float texBaseX = sprite_->GetAtlasX();
  float texBaseY = sprite_->GetAtlasY();
  float texWidth = sprite_->GetAtlasW();
  float texHeight = sprite_->GetAtlasH();

  float xGap = globalXGap;
  float zGap = globalZGap;

  v1.texCoord2(texBaseX, texBaseY);
  v1.vertex3(-cubeWidth_/2-xGap, cubeHeight_/2, -cubeDepth_-zGap);

  v2.texCoord2(texBaseX + texWidth, texBaseY);
  v2.vertex3(cubeWidth_/2+xGap, cubeHeight_/2, -cubeDepth_-zGap);		

  v3.texCoord2(texBaseX + texWidth, texBaseY + texHeight);
  v3.vertex3(cubeWidth_/2+xGap, cubeHeight_/2, 0);

  v4.texCoord2(texBaseX, texBaseY + texHeight);
  v4.vertex3(-cubeWidth_/2-xGap, cubeHeight_/2, 0);
}
void BookPaperBuilder::createForwardModel() 
{
  float texBaseX = sprite_->GetAtlasX();
  float texBaseY = sprite_->GetAtlasY();
  float texWidth = sprite_->GetAtlasW();
  float texHeight = sprite_->GetAtlasH();

  float xGap = globalXGap;
  float yGap = globalYGap;

  v1.texCoord2(texBaseX, texBaseY+texHeight);
  v1.vertex3(-cubeWidth_/2-xGap, -cubeHeight_/2-yGap, -cubeDepth_);

  v2.texCoord2(texBaseX + texWidth, texBaseY+texHeight);
  v2.vertex3(cubeWidth_/2+xGap, -cubeHeight_/2-yGap, -cubeDepth_);		

  v3.texCoord2(texBaseX + texWidth, texBaseY);
  v3.vertex3(cubeWidth_/2+xGap, cubeHeight_/2+yGap, -cubeDepth_);		

  v4.texCoord2(texBaseX, texBaseY);
  v4.vertex3(-cubeWidth_/2-xGap, cubeHeight_/2+yGap, -cubeDepth_);
}

void BookPaperBuilder::createNormalModel(float width, float height) 
{
  SR_ASSERT(sprite_ != NULL);

  //스프라이트의 크기와 크기비율을 곱해서 적절한 모델 크기를 만든다 
  width = sprite_->w * width;
  height = sprite_->h * height;

  //float texBaseX = sprite_->getAtlasX();
  //float texBaseY = sprite_->getAtlasY();
  //float texWidth = sprite_->getAtlasWidth();
  //float texHeight = sprite_->getAtlasHeight();
  ///원래크기보다 약간 더 작게 그려야 위로 뺴져나가는게 없다
  ///@bug 정확한 텍스쳐 좌표로 그릴떄 뺴져나가는게 버그듯. 이부분을 gl설정어딘가에서 잘 고치면 
  ///될거같긴한데..
  /*
  그리는 텍스쳐 자체를 조정하는것
  texBaseX += texWidth * 0.01;
  texBaseY += texHeight * 0.01;
  texWidth *= 0.99;
  texHeight *= 0.99;
  */

  //http://zarpar.tistory.com/45
  /*
  If enabled, the texture coordinates will be calculated by using this formula:
  로 하면 고칠수 있을듯?
  - texCoord.left = (rect.origin.x*2+1) / (texture.wide*2);
  - texCoord.right = texCoord.left + (rect.size.width*2-2)/(texture.wide*2);
  */

  float tex_width = sprite_->parent->tex->tex_header.tex_width;
  float tex_height = sprite_->parent->tex->tex_header.tex_height;

  float texLeft = (float)(sprite_->x * 2 + 1) / (tex_width * 2);
  float texRight = texLeft + (float)(sprite_->w * 2 - 2) / (tex_width * 2);
  float texTop = (float)(sprite_->y * 2 + 1) / (tex_height * 2);
  float texBottom = texTop + (float)(sprite_->h * 2 - 2) / (tex_height * 2);
  /*
  //left bottom
  builder.texCoord2(texBaseX, texBaseY+texHeight);
  builder.vertex3(-width/2, -height/2, 0);
  //right bottom
  builder.texCoord2(texWidth+texBaseX, texBaseY+texHeight);
  builder.vertex3(width/2, -height/2, 0);	
  //right top
  builder.texCoord2(texWidth+texBaseX, texBaseY);
  builder.vertex3(width/2, height/2, 0);	
  //left top
  builder.texCoord2(texBaseX, texBaseY);
  builder.vertex3(-width/2, height/2, 0);
  */
  //left bottom
  v1.texCoord2(texLeft, texBottom);
  v1.vertex3(-width/2, -height/2, 0);
  //right bottom
  v2.texCoord2(texRight, texBottom);
  v2.vertex3(width/2, -height/2, 0);	
  //right top
  v3.texCoord2(texRight, texTop);
  v3.vertex3(width/2, height/2, 0);	
  //left top
  v4.texCoord2(texLeft, texTop);
  v4.vertex3(-width/2, height/2, 0);
}


BookPaperPtr BookPaperBuilder::build()
{	
  BookPaper *paper = NULL;
  switch(type_) {
  case BookPaperTypeNormal:
    paper = new BookPaper(sprite_, type_);
    paper->v1 = v1;
    paper->v2 = v2;
    paper->v3 = v3;
    paper->v4 = v4;

    paper->setPos(x_, y_, z_);
    paper->setRotate(pitch_, roll_, yaw_);
    break;
  case BookPaperTypeFloor:
  case BookPaperTypeLeft:
  case BookPaperTypeRight:
  case BookPaperTypeCeil:
  case BookPaperTypeForward:
    paper = new BookPaper(sprite_, type_);
    paper->v1 = v1;
    paper->v2 = v2;
    paper->v3 = v3;
    paper->v4 = v4;

    break;
  case BookPaperTypeFoldAxisY:
    SR_ASSERT(false);
    break;
  }

  clear();

  return BookPaperPtr(paper);
}

BookPaperBuilder& BookPaperBuilder::setType(BookPaperType type)
{
  type_ = type;
  return *this;
}

BookPaperBuilder& BookPaperBuilder::setPos(const sora::vec3 &pos)
{
  x_ = pos.x;
  y_ = pos.y;
  z_ = pos.z;
  return *this;
}

BookPaperBuilder& BookPaperBuilder::setFold(AxisDirection axis, float degree)
{
  SR_ASSERT(axis == AxisDirectionY && "only support y axis");
  axis_ = axis;
  angle_ = degree;
  return *this;
}

BookPaperBuilder& BookPaperBuilder::setRotate(float pitch, float roll, float yaw)
{
  pitch_ = pitch;
  roll_ = roll;
  yaw_ = yaw;
  return *this;
}
BookPaperBuilder& BookPaperBuilder::setSprite(TextureAtlasSegment *sprite)
{
  sprite_ = sprite;
  return *this;
}
void BookPaperBuilder::clear()
{
  sprite_ = NULL;
  type_ = BookPaperTypeNormal;

  pitch_ = 0;
  roll_ = 0;
  yaw_ = 0;

  x_ = 0;
  y_ = 0;
  //z_ = 0;

  width_ = 1;
  height_ = 1;

  axis_ = AxisDirectionY;
  angle_ = 0;
}

float BookPaperBuilder::getCubeWidth() const
{
  return cubeWidth_;
}
float BookPaperBuilder::getCubeHeight() const
{
  return cubeHeight_;
}
float BookPaperBuilder::getCubeDepth() const
{
  return cubeDepth_;
}
}