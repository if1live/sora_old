// Ŭnicode please
#include "yukino_stdafx.h"
#include "book_paper.h"
#include "book_paper_builder.h"
#include "book.h"
#include "sora/texture_atlas.h"
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

BookPaperBuilder::BookPaperBuilder(float cube_width, float cube_height, float cube_depth)
  : cube_width_(cube_width), cube_height_(cube_height), cube_depth_(cube_depth) {
  SR_ASSERT(cube_width_ > 0 && cube_height_ > 0 && cube_depth_ > 0);
  Clear();
}

BookPaperBuilder::~BookPaperBuilder() {
}

void BookPaperBuilder::CreateFloorModel() {
  //string cubeFile = Path::appPath("res/floor01.jpg");
  //string cubeFile = Path::appPath("res/img_test.bmp");
  //string cubeFile = Path::appPath("res/small_test_2x2.png");
  //string cubeFile = Path::appPath("res/small_test_2x1.png");
  //TextureDescPtr texDesc = ImageLoader::loader().loadJpg2Tex(cubeFile);
  //TexturePtr tex = TextureManager::GetInstance().Create(texDesc, string("floor"));

  /*cube model 적당히 생성*/
  /*모델을 생성*/

  //texture atlas를 쓸지도 몰라서 테스트로 짰던 코드이다. 실제로는 결국 안썼다
  //float baseX = 0;
  //float baseY = (float)(tex->getDesc().texHeight - tex->getDesc().origHeight) / tex->getDesc().texHeight;
  //float width = (float)(tex->getDesc().origWidth) / tex->getDesc().texWidth;
  //float height = 1.0 - baseY;
  float texBaseX = sprite_->GetTexCoordX();
  float texBaseY = sprite_->GetTexCoordY();
  float texWidth = sprite_->GetTexCoordWidth();
  float texHeight = sprite_->GetTexCoordHeight();

  float xGap = globalXGap;
  float zGap = globalZGap;

  //gap=크기*0.01		
  v1.s = texBaseX;
  v1.t = texBaseY+texHeight;
  v1.x = -cube_width_/2-xGap;
  v1.y = -cube_height_/2;
  v1.z = 0;

  v2.s = texBaseX + texWidth;
  v2.t = texBaseY+texHeight;
  v2.x = cube_width_/2+xGap;
  v2.y = -cube_height_/2;
  v2.z = 0;

  v3.s = texBaseX + texWidth;
  v3.t = texBaseY;
  v3.x = cube_width_/2+xGap;
  v3.y = -cube_height_/2;
  v3.z = -cube_depth_-zGap;		

  v4.s = texBaseX;
  v4.t = texBaseY;
  v4.x = -cube_width_/2-xGap;
  v4.y = -cube_height_/2;
  v4.z = -cube_depth_-zGap;
}

void BookPaperBuilder::CreateLeftModel() {
  float texBaseX = sprite_->GetTexCoordX();
  float texBaseY = sprite_->GetTexCoordY();
  float texWidth = sprite_->GetTexCoordWidth();
  float texHeight = sprite_->GetTexCoordHeight();

  //좌우벽은 gap를 신경쓰지 말자
  float yGap = 0;
  float zGap = 0;

  v1.s = texBaseX;
  v1.t = texBaseY+texHeight;
  v1.Vertex3(-cube_width_/2, -cube_height_/2-yGap, 0);

  v2.TexCoord2(texBaseX + texWidth, texBaseY+texHeight);
  v2.Vertex3(-cube_width_/2, -cube_height_/2-yGap, -cube_depth_-zGap);		

  v3.TexCoord2(texBaseX + texWidth, texBaseY);
  v3.Vertex3(-cube_width_/2, cube_height_/2+yGap, -cube_depth_-zGap);		

  v4.TexCoord2(texBaseX, texBaseY);
  v4.Vertex3(-cube_width_/2, cube_height_/2+yGap, 0);
}
void BookPaperBuilder::CreateRightModel()  {
  float texBaseX = sprite_->GetTexCoordX();
  float texBaseY = sprite_->GetTexCoordY();
  float texWidth = sprite_->GetTexCoordWidth();
  float texHeight = sprite_->GetTexCoordHeight();

  //좌우벽은 gap를 신경쓰지 말자
  float yGap = 0;
  float zGap = 0;

  v1.TexCoord2(texBaseX, texBaseY+texHeight);
  v1.Vertex3(cube_width_/2, -cube_height_/2-yGap, -cube_depth_-zGap);

  v2.TexCoord2(texBaseX + texWidth, texBaseY+texHeight);
  v2.Vertex3(cube_width_/2, -cube_height_/2-yGap, 0);		

  v3.TexCoord2(texBaseX + texWidth, texBaseY);
  v3.Vertex3(cube_width_/2, cube_height_/2+yGap, 0);		

  v4.TexCoord2(texBaseX, texBaseY);
  v4.Vertex3(cube_width_/2, cube_height_/2+yGap, -cube_depth_-zGap);
}
void BookPaperBuilder::CreateCeilModel() {
  float texBaseX = sprite_->GetTexCoordX();
  float texBaseY = sprite_->GetTexCoordY();
  float texWidth = sprite_->GetTexCoordWidth();
  float texHeight = sprite_->GetTexCoordHeight();

  float xGap = globalXGap;
  float zGap = globalZGap;

  v1.TexCoord2(texBaseX, texBaseY);
  v1.Vertex3(-cube_width_/2-xGap, cube_height_/2, -cube_depth_-zGap);

  v2.TexCoord2(texBaseX + texWidth, texBaseY);
  v2.Vertex3(cube_width_/2+xGap, cube_height_/2, -cube_depth_-zGap);		

  v3.TexCoord2(texBaseX + texWidth, texBaseY + texHeight);
  v3.Vertex3(cube_width_/2+xGap, cube_height_/2, 0);

  v4.TexCoord2(texBaseX, texBaseY + texHeight);
  v4.Vertex3(-cube_width_/2-xGap, cube_height_/2, 0);
}
void BookPaperBuilder::CreateForwardModel() {
  float texBaseX = sprite_->GetTexCoordX();
  float texBaseY = sprite_->GetTexCoordY();
  float texWidth = sprite_->GetTexCoordWidth();
  float texHeight = sprite_->GetTexCoordHeight();

  float xGap = globalXGap;
  float yGap = globalYGap;

  v1.TexCoord2(texBaseX, texBaseY+texHeight);
  v1.Vertex3(-cube_width_/2-xGap, -cube_height_/2-yGap, -cube_depth_);

  v2.TexCoord2(texBaseX + texWidth, texBaseY+texHeight);
  v2.Vertex3(cube_width_/2+xGap, -cube_height_/2-yGap, -cube_depth_);		

  v3.TexCoord2(texBaseX + texWidth, texBaseY);
  v3.Vertex3(cube_width_/2+xGap, cube_height_/2+yGap, -cube_depth_);		

  v4.TexCoord2(texBaseX, texBaseY);
  v4.Vertex3(-cube_width_/2-xGap, cube_height_/2+yGap, -cube_depth_);
}

void BookPaperBuilder::CreateNormalModel(float width, float height) {
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

  float tex_width = sprite_->GetTexWidth();
  float tex_height = sprite_->GetTexHeight();
  
  float texLeft = (float)(sprite_->x * 2 + 1) / (tex_width * 2);
  float texRight = texLeft + (float)(sprite_->w * 2 - 2) / (tex_width * 2);
  float texTop = (float)(sprite_->y * 2 + 1) / (tex_height * 2);
  float texBottom = texTop + (float)(sprite_->h * 2 - 2) / (tex_height * 2);
  /*
  //left bottom
  builder.TexCoord2(texBaseX, texBaseY+texHeight);
  builder.Vertex3(-width/2, -height/2, 0);
  //right bottom
  builder.TexCoord2(texWidth+texBaseX, texBaseY+texHeight);
  builder.Vertex3(width/2, -height/2, 0);	
  //right top
  builder.TexCoord2(texWidth+texBaseX, texBaseY);
  builder.Vertex3(width/2, height/2, 0);	
  //left top
  builder.TexCoord2(texBaseX, texBaseY);
  builder.Vertex3(-width/2, height/2, 0);
  */
  //left bottom
  v1.TexCoord2(texLeft, texBottom);
  v1.Vertex3(-width/2, -height/2, 0);
  //right bottom
  v2.TexCoord2(texRight, texBottom);
  v2.Vertex3(width/2, -height/2, 0);	
  //right top
  v3.TexCoord2(texRight, texTop);
  v3.Vertex3(width/2, height/2, 0);	
  //left top
  v4.TexCoord2(texLeft, texTop);
  v4.Vertex3(-width/2, height/2, 0);
}

BookPaper BookPaperBuilder::Build() {
  BookPaper paper;
  Build(&paper);
  return paper;
}

void BookPaperBuilder::Build(BookPaper *paper) {	
  switch(type_) {
  case kBookPaperNormal:
    paper->SetBaseAttribute(sprite_, type_);
    paper->v1 = v1;
    paper->v2 = v2;
    paper->v3 = v3;
    paper->v4 = v4;

    paper->pos_x = x_;
    paper->pos_y = y_;
    paper->pos_z = z_;
    paper->pitch = pitch_;
    paper->roll = roll_;
    paper->yaw = yaw_;
    break;
  case kBookPaperFloor:
  case kBookPaperLeft:
  case kBookPaperRight:
  case kBookPaperCeil:
  case kBookPaperForward:
    paper->SetBaseAttribute(sprite_, type_);
    paper->v1 = v1;
    paper->v2 = v2;
    paper->v3 = v3;
    paper->v4 = v4;

    break;
  case kBookPaperFoldAxisY:
    SR_ASSERT(false);
    break;
  }
  Clear();
}

BookPaperBuilder& BookPaperBuilder::set_type(BookPaperType type) {
  type_ = type;
  return *this;
}

BookPaperBuilder& BookPaperBuilder::set_pos(const sora::vec3 &pos) {
  x_ = pos.x;
  y_ = pos.y;
  z_ = pos.z;
  return *this;
}

BookPaperBuilder& BookPaperBuilder::set_fold(AxisDirection axis, float degree) {
  SR_ASSERT(axis == kAxisDirectionY && "only support y axis");
  axis_ = axis;
  angle_ = degree;
  return *this;
}

BookPaperBuilder& BookPaperBuilder::set_rotate(float pitch, float roll, float yaw) {
  pitch_ = pitch;
  roll_ = roll;
  yaw_ = yaw;
  return *this;
}
BookPaperBuilder& BookPaperBuilder::set_sprite(TextureSubImage *sprite)
{
  sprite_ = sprite;
  return *this;
}
void BookPaperBuilder::Clear() {
  sprite_ = NULL;
  type_ = kBookPaperNormal;

  pitch_ = 0;
  roll_ = 0;
  yaw_ = 0;

  x_ = 0;
  y_ = 0;
  //z_ = 0;

  width_ = 1;
  height_ = 1;

  axis_ = kAxisDirectionY;
  angle_ = 0;
}
/////////////////////////////////

BookPaper BookPaperBuilder::CreateFloor(TextureSubImage *sprite) {
  Clear();
  set_sprite(sprite);
  set_type(kBookPaperFloor);
  CreateFloorModel();
  return Build();
}

BookPaper BookPaperBuilder::CreateLeft(TextureSubImage *sprite) {
  Clear();
  set_sprite(sprite);
  set_type(kBookPaperLeft);
  CreateLeftModel();
  return Build();
}
BookPaper BookPaperBuilder::CreateRight(TextureSubImage *sprite) {
  Clear();
  set_sprite(sprite);
  set_type(kBookPaperRight);
  CreateRightModel();
  return Build();
}
BookPaper BookPaperBuilder::CreateCeil(TextureSubImage *sprite) {
  Clear();
  set_sprite(sprite);
  set_type(kBookPaperCeil);
  CreateCeilModel();
  return Build();
}
BookPaper BookPaperBuilder::CreateForward(TextureSubImage *sprite) {
  Clear();
  set_sprite(sprite);
  set_type(kBookPaperForward);
  CreateForwardModel();
  return Build();
}
BookPaper BookPaperBuilder::CreateNormal(TextureSubImage *sprite, float x, float y, float z, float w, float h) {
  Clear();
  set_sprite(sprite);
  set_type(kBookPaperNormal);
  set_pos(vec3(x, y, z));
  CreateNormalModel(w, h);
  return Build();
}

BookPaper BookPaperBuilder::CreateNormalWithRoll(TextureSubImage *sprite, float x, float y, float z, float w, float h, float roll) {
  Clear();
  set_sprite(sprite);
  set_type(kBookPaperNormal);
  set_pos(vec3(x, y, z));
  CreateNormalModel(w, h);
  set_rotate(0, roll, 0);
  return Build();
}

BookPaper BookPaperBuilder::CreateNormalWithPitch(TextureSubImage *sprite, float x, float y, float z, float w, float h, float pitch) {
  Clear();
  set_sprite(sprite);
  set_type(kBookPaperNormal);
  set_pos(vec3(x, y, z));
  CreateNormalModel(w, h);
  set_rotate(pitch, 0, 0);
  return Build();
}
BookPaper BookPaperBuilder::CreateNormalWithYaw(TextureSubImage *sprite, float x, float y, float z, float w, float h, float yaw) {
  Clear();
  set_sprite(sprite);
  set_type(kBookPaperNormal);
  set_pos(vec3(x, y, z));
  CreateNormalModel(w, h);
  set_rotate(0, 0, yaw);
  return Build();
}
}