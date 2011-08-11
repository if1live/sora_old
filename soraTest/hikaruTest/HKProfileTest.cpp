#include "../SoraTestStdAfx.h"
#include "hikaru/HKProfileSample.h"

using namespace std;
using namespace hikaru;

TEST(Prifle, test)
{
	clock_t t = clock();
	cout << t << endl;
	//time test
	//for(int i = 0 ; i < 1000000000 ; i++) {
	//	static int a = 0;
	//	a++;
	//}

	clock_t dt = t - clock();
	cout << dt << endl;
}