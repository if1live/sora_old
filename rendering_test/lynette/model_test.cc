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
#include "precompile.h"
#include "lynette/model.h"

using lynette::Model;

TEST(LNModel, getStrideAndIsUseXX) {
	//pos
	Model m1(lynette::LYNETTE_TRIANGLES);
	EXPECT_FALSE(m1.isUseColor());
	EXPECT_FALSE(m1.isUseTexCoord());
	EXPECT_FALSE(m1.isUseNormal());
	EXPECT_TRUE(3*sizeof(float) == m1.getStride());

	//pos + color
	Model m2(lynette::LYNETTE_TRIANGLES, lynette::LYNETTE_USE_COLOR);
	EXPECT_TRUE(m2.isUseColor());
	EXPECT_FALSE(m2.isUseTexCoord());
	EXPECT_FALSE(m2.isUseNormal());
	EXPECT_EQ(7*sizeof(float), (unsigned int)m2.getStride());
	
	//pos + tex + normal
	Model m3(lynette::LYNETTE_TRIANGLES, lynette::LYNETTE_USE_TEXCOORD | lynette::LYNETTE_USE_NORMAL);
	EXPECT_FALSE(m3.isUseColor());
	EXPECT_TRUE(m3.isUseTexCoord());
	EXPECT_TRUE(m3.isUseNormal());
	EXPECT_EQ(8*sizeof(float), (unsigned int)m3.getStride());
	
	//pos + tex
	Model m4(lynette::LYNETTE_LINES, lynette::LYNETTE_USE_TEXCOORD);
	EXPECT_FALSE(m4.isUseColor());
	EXPECT_TRUE(m4.isUseTexCoord());
	EXPECT_FALSE(m4.isUseNormal());
	EXPECT_EQ(5*sizeof(float), (unsigned int)m4.getStride());
	
}
