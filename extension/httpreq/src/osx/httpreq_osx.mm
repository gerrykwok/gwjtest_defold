#if defined(DM_PLATFORM_OSX)

#include "../httpreq.h"

static NSString* runShellCommand(NSString *path, NSArray *arguments)
{
	NSTask       *task;
	NSPipe       *pipe;
	NSFileHandle *file;
	NSData       *dataRead;

	task = [[NSTask alloc] init];
	[task setLaunchPath: path];
	[task setArguments: arguments];

	pipe = [NSPipe pipe];
	[task setStandardOutput: pipe];

	file = pipe.fileHandleForReading;

	[task launch];
	//	[task waitUntilExit];//这句会造成程序阻塞死锁

	dataRead = [file readDataToEndOfFile];
	return [[NSString alloc] initWithData: dataRead encoding: NSUTF8StringEncoding];
}

int httpreq_isCharlesRunning()
{
	NSString *ret = runShellCommand(@"/bin/ps", [NSArray arrayWithObjects: @"-A", nil]);
	NSRange range = [ret rangeOfString:@"charles" options:NSCaseInsensitiveSearch];
	if(range.location != NSNotFound)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#endif