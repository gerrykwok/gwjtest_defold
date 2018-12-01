#if defined(DM_PLATFORM_IOS)

#import "myclass.h"

@implementation MyClass

+(NSString*)test1:(NSDictionary*)params
{
	return @"my str";
}

+(BOOL)test2:(NSDictionary*)params
{
	return YES;
}

+(char)test3:(NSDictionary*)params
{
	return 'a';
}

+(int)test4:(NSDictionary*)params
{
	return 123;
}

+(float)test5:(NSDictionary*)params
{
	return 456.789;
}

@end

#endif
