#import "AKTest.h"

int main(int argc, char *argv[]) {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	int result = RUL_ALL_TESTS();

    [pool release];
	return result;
}

