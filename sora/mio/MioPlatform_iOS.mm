#include "mio/MioPlatform_iOS.h"

#import <Foundation/Foundation.h>

using namespace std;

/*
 #define LOG(format, ...)	\
 CFShow([NSString stringWithFormat:[NSString stringWithUTF8String:format], ## __VA_ARGS__]);
 //워닝안뜨는 한줄로그
 #define LINE_LOG(msg)	CFShow([NSString stringWithUTF8String:msg]);
 */


namespace mio 
{
	std::string PathCoreIOS::docPath(const std::string &file)
	{
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *docDir = [paths objectAtIndex:0];
		
		NSString *nsfilename = [NSString stringWithCString:file.c_str() encoding:NSUTF8StringEncoding];
		NSString *path = [docDir stringByAppendingPathComponent:nsfilename];
		std::string result = [path cStringUsingEncoding:NSUTF8StringEncoding];
		[pool drain];
		return result;
	}
	std::string PathCoreIOS::appPath(const std::string &file)
	{
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSString *filenameStr = [NSString stringWithCString:file.c_str() encoding:NSUTF8StringEncoding];
		NSString *path = [[NSBundle mainBundle] pathForResource:filenameStr ofType:nil];
		
		const char *result = NULL;
		if([path length] == 0)
		{
			result = "";
		}
		else
		{
			result = [path cStringUsingEncoding:NSUTF8StringEncoding];
		}
		string resultStr(result);
		[pool drain];
		return resultStr;
	}
	bool PathCoreIOS::isFileExist(const std::string &file)
	{
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
		NSString *path = [NSString stringWithCString:file.c_str() encoding:NSUTF8StringEncoding];
		BOOL exist = [[NSFileManager defaultManager] fileExistsAtPath:path];
		[pool drain];
		if(exist == YES)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void ConsoleCoreIOS::log(const std::string &msg) const
	{
#define LINE_LOG(msg)	CFShow([NSString stringWithUTF8String:msg.c_str()]);
		LINE_LOG(msg);
#undef LINE_LOG
	}
	void ConsoleCoreIOS::timelog(const std::string &msg) const
	{
		NSLog(@"%s", msg.c_str());
	}
}