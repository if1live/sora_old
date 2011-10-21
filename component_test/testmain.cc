// Ŭnicode please 
#include "precompile.h"

#if SR_WIN
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif

int main(int argc, char *argv[]) {
#if SR_WIN
  ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  ::testing::InitGoogleTest(&argc, argv);
  int result = RUN_ALL_TESTS();
  getchar();
  return result;
}