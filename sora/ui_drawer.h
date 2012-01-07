/*  Copyright (C) 2011 by if1live */
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
#ifndef SORA_UI_DRAWER_H_
#define SORA_UI_DRAWER_H_

namespace sora {;

class ImageLabel;
class Button;
class UIContainer;
class UIComponent;
class TextureSubImage;

class UIDrawer {
public:
	UIDrawer() {}
	~UIDrawer() {}

  void DrawRoot(UIComponent *root);

	void Draw(ImageLabel *label);
	void Draw(Button *btn);

	///@brief hud는 그릴때 행렬이 바뀌니까 잘 저장했다 복구해야한다&
	void BeforeDraw();
	void AfterDraw();
	void DrawTouchArea(UIContainer *container);

private:
  void Draw(TextureSubImage *img);
};

}

#endif