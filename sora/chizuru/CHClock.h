#pragma once

namespace chizuru
{
	///@brief stopwatch
	class Stopwatch {
	public:
		Stopwatch();

		void start();
		unsigned int stop();
		unsigned int getTime();

	private:
		unsigned int _start;
		unsigned int _result;
	};
	
}
