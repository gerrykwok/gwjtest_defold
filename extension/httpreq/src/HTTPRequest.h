#ifndef __HTTPREQUEST_H__
#define __HTTPREQUEST_H__

#include <string>
#include <vector>
#include <map>
#include <functional>

#ifdef GEN_BINDING
typedef int LUA_FUNCTION;
typedef int LUA_STRING;
typedef int LUA_BUFFER;
typedef void CURL;
typedef unsigned int size_t;
typedef int curl_httppost;
#else
#include "httpreq.h"
#if defined(DM_PLATFORM_ANDROID)
#include "curl/android/curl/curl.h"
#elif defined(DM_PLATFORM_WINDOWS)
#include "curl/win32/curl/curl.h"
#elif defined(DM_PLATFORM_OSX)
#include "curl/curl.h"
#elif defined(DM_PLATFORM_IOS)
#include "curl/ios/curl/curl.h"
#endif
#endif

#define kCCHTTPRequestStateIdle                 0
#define kCCHTTPRequestStateCleared              1
#define kCCHTTPRequestStateInProgress           2
#define kCCHTTPRequestStateCompleted            3
#define kCCHTTPRequestStateCancelled            4
#define kCCHTTPRequestStateFailed               5

#define kCCHTTPRequestCURLStateIdle             0
#define kCCHTTPRequestCURLStateBusy             1
#define kCCHTTPRequestCURLStateClosed           2

class HTTPRequest;

typedef std::vector<std::string> HTTPRequestHeaders;
typedef HTTPRequestHeaders::iterator HTTPRequestHeadersIterator;
typedef std::function<void(const char*, HTTPRequest*)> HTTPRequestCallback;

class HTTPRequest
{
public:
	static HTTPRequest* create(LUA_FUNCTION listener, const char *url, const char *method);
#ifndef GEN_BINDING
	static HTTPRequest* create(HTTPRequestCallback callback, const char *url, const char *method);
#endif
	void addRequestHeader(const char *header);
	void addPOSTValue(const char *key, const char *value);
	void setPOSTData(const char *data, size_t len = 0);
	void setCookieString(const char *cookie);
	const std::string getCookieString(void);
	void setTimeout(int timeout);
	void setConnectTimeout(int timeout);
	void setReadTimeout(int timeout);
	void start();
	void cancel();
	int getState(void);
	int getResponseStatusCode(void);
	const std::string getResponseHeadersString(void);
#ifndef GEN_BINDING
	const HTTPRequestHeaders& getResponseHeaders(void);
#endif
	const std::string getResponseString(void);
	//void *getResponseData(void);
	LUA_BUFFER getResponseDataLua(void);
	int getResponseDataLength(void);
	size_t saveResponseData(const char *filename);
	int getErrorCode(void);
	const std::string getErrorMessage(void);
protected:
	void release();
	bool initWithListener(LUA_FUNCTION listener, const char *url, const char *method);
	bool initWithCallback(HTTPRequestCallback callback, const char *url, const char *method);
	bool initWithUrl(const char *url, const char* method);
	void onRequest(void);
	size_t onWriteData(void *buffer, size_t bytes);
	size_t onWriteHeader(void *buffer, size_t bytes);
	int onProgress(double dltotal, double dlnow, double ultotal, double ulnow);
	void cleanup(void);
	void update();

	static size_t writeDataCURL(void *buffer, size_t size, size_t nmemb, void *userdata);
	static size_t writeHeaderCURL(void *buffer, size_t size, size_t nmemb, void *userdata);
	static int progressCURL(void *userdata, double dltotal, double dlnow, double ultotal, double ulnow);
	static bool isCharlesRunning();
private:
	HTTPRequest();
	~HTTPRequest();

	std::string m_url;
	HTTPRequestCallback m_callback;
	int m_listener;
	int m_curlState;

	CURL *m_curl;
	curl_httppost *m_formPost;
	curl_httppost *m_lastPost;

	int     m_state;
	int     m_errorCode;
	std::string  m_errorMessage;

	// request
	typedef std::map<std::string, std::string> Fields;
	Fields m_postFields;
	HTTPRequestHeaders m_headers;

	void* m_postData;
	size_t m_postDataLen;

	// response
	long m_responseCode;
	HTTPRequestHeaders m_responseHeaders;
	void *m_responseBuffer;
	size_t m_responseBufferLength;
	size_t m_responseDataLength;
	std::string m_responseCookies;

	double m_dltotal;
	double m_dlnow;
	double m_ultotal;
	double m_ulnow;

	unsigned int m_idSchedule;

#if defined(DM_PLATFORM_WINDOWS)
	static DWORD WINAPI requestCURL(LPVOID userdata);
#else
	static void *requestCURL(void *userdata);
#endif
};

#endif
