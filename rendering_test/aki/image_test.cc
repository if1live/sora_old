#include "precompile.h"
#include "aki/image.h"
#if _WIN_
#include "aki/image_loader_soil.h"
#endif
#include "mio/MioPath.h"


TEST(AkiImage, test)
{
}

TEST(AkiImageLoader_SOIL, load)
{
	string path1 = Path::appPath("testdata/aki/c.png");
	ImagePtr img = ImageLoader_SOIL::load(path1);
	const ImageDescription &desc1 = img->getDesc();
	EXPECT_EQ(InternalFormatRGBA, desc1.getInternalFormat());
	EXPECT_EQ(500, desc1.getWidth());
	EXPECT_EQ(282, desc1.getHeight());

	string path2 = Path::appPath("testdata/aki/target.png");
	ImagePtr img1 = ImageLoader_SOIL::load(path2);
	const ImageDescription &desc2 = img1->getDesc();
	EXPECT_EQ(InternalFormatRGBA, desc2.getInternalFormat());
	EXPECT_EQ(256, desc2.getWidth());
	EXPECT_EQ(256, desc2.getHeight());
}