// Ŭnicode please
#pragma once
#if _WIN_
namespace chizuru
{
	class KeyboardData {
	public:
		KeyboardData(unsigned char key);
		unsigned char key;
		
		static KeyboardData getLast();
	private:
		static unsigned char lastKey_;
	};
}
#endif