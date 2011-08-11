// Ŭnicode please
#pragma once

namespace lynette
{
	class Model;
	typedef std::tr1::shared_ptr<Model> ModelPtr;

	template<typename T> class BoundingVolumeTpl;
	typedef BoundingVolumeTpl<float> BoundingVolume;
	typedef BoundingVolumeTpl<float> BoundingVolumeF;
	typedef BoundingVolumeTpl<double> BoundingVolumeD;

	enum {
		LYNETTE_USE_COLOR = 0x01,
		LYNETTE_USE_TEXCOORD = 0x02,
		LYNETTE_USE_NORMAL = 0x04
	};

	enum {
		LYNETTE_POINTS,
		LYNETTE_LINES,
		LYNETTE_LINE_STRIP,
		LYNETTE_LINE_LOOP,
		LYNETTE_TRIANGLES,
		LYNETTE_TRIANGLE_FAN,
		LYNETTE_TRIANGLE_STRIP,
		LYNETTE_QUADS,
	};
}