// Ŭnicode please
#pragma once

#include "lynette/LNTypedef.h"

namespace lynette
{
	template<typename T>
	class BoundingVolumeTpl {
	public:
		BoundingVolumeTpl(const sora::model::Model &model);
		~BoundingVolumeTpl();

		T getLeft() const;
		T getRight() const;
		T getTop() const;
		T getBottom() const;
		T getFront() const;
		T getBack() const;
		T getWidth() const;
		T getHeight() const;
		T getDepth() const;
	private:
		T left_;
		T right_;
		T top_;
		T bottom_;
		T front_;
		T back_;
	};
}

//template impl 
#include "lynette/LNBoundingVolumeTpl.hpp"