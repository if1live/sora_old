/*  Copyright (C) 2011-2012 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#ifndef SORA_SELECTION_BUFFER_H_
#define SORA_SELECTION_BUFFER_H_

namespace sora {;

class SelectionBuffer;
class SelectionRequest;
class ShaderProgram;

//물체 선택하기 위한 selection buffer
//reference
//http://content.gpwiki.org/index.php/OpenGL_Using_Unique_Color_IDs
class SelectionBuffer {
public:
  friend class SelectionRequest;
  SelectionBuffer();
  ~SelectionBuffer();

  void Init(int w, int h);
  void Deinit();

  static void IdToArray(int color_id, int arr[4]);

  ShaderProgram *shader() { return selection_shader_; }
  int GetColorIdLocation() const;
  
private:
  void BeginDraw(int gl_x, int gl_y);
  //적절히 물체를 selection buffer에 그린다.
  //u_colorId에다가 갖이 넣고 그려야하는데
  //이것까지 연결하기위해서는 완전한 객체 시스템이 있어야될거같아서
  //일단 하지 않았다
  int EndDraw(int gl_x, int gl_y);

private:
  GLuint fbo_;
  GLuint color_;
  GLuint depth_;
  //크기는 창과 동일
  int width_;
  int height_;

  ShaderProgram *selection_shader_;
};

//RAII의 구현을 위해서 사용
//opengl 상태 변화를 맞추는데 쓰인다
class SelectionRequest {
public:
  SelectionRequest(SelectionBuffer *buf, int gl_x, int gl_y);
  ~SelectionRequest();
  int GetId();
private:
  SelectionBuffer *buffer_;
  int gl_x_;
  int gl_y_;

};

}

#endif  // SORA_SELECTION_BUFFER_H_