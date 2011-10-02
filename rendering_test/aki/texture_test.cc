#include "../SoraTestStdAfx.h"
#include "aki/AkiTexture.h"
#include "aki/AkiImage.h"
#include "mio/MioPath.h"

using namespace std;
using namespace aki;
using namespace mio;

TEST(AkiTexture, test)
{
	//load sample image
	string path1 = Path::appPath("testdata/aki/c.png");
	ImagePtr img = Image::create(path1);
	
	Texture tex;
	tex.loadImage(img.get());
	//원본 이미지 크기 정보
	//EXPECT_EQ(500, desc1.getWidth());
	//EXPECT_EQ(282, desc1.getHeight());

	const TextureSize &size = tex.getSize();
	EXPECT_EQ(500, size.getOrigWidth());
	EXPECT_EQ(282, size.getOrigHeight());
	EXPECT_EQ(512, size.getTexWidth());
	EXPECT_EQ(512, size.getTexHeight());
}