// Ŭnicode please 
#include "precompile.h"

#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>

void use_atexit() {
  ::_CrtDumpMemoryLeaks();
  printf("using.. atexit function...\n");
}

int main(int argc, char *argv[]) {
  atexit( use_atexit );
  ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	getchar();
	return result;
}