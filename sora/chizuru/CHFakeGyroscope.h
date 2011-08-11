// Ŭnicode please
#pragma once

#include "matsu/MTSingleton.h"
#include "matsu/MTVector.h"

namespace chizuru
{

	class FakeGyroscope : public matsu::Singleton<FakeGyroscope> {
	public:
		matsu::vec3 calc(const matsu::vec3 &accelVec, const matsu::vec3 &headingVec) const;
	};
}