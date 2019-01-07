#include "HTTPRequest.h"
#include "httpreq.h"
#if defined(DM_PLATFORM_WINDOWS)
#include <TlHelp32.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi")
#endif
#include <sstream> //for std::stringbuf

#define DEFAULT_TIMEOUT			10 // 10 seconds
#define BUFFER_CHUNK_SIZE 		32768 // 32 KB
int httpreq_isCharlesRunning();
std::string curl_proxy = "";

void httpreq_printCurlVersion()
{
	dmLogInfo("curl version:%s", curl_version());
}

HTTPRequest* HTTPRequest::create(LUA_FUNCTION listener, const char *url, const char *method)
{
	//dmLogInfo("http create,listener=%d,url=%s,method=%s", listener, url, method);
	HTTPRequest *request = new HTTPRequest();
	request->initWithListener(listener, url, method);
	return request;
}

bool HTTPRequest::initWithListener(LUA_FUNCTION listener, const char *url, const char *method)
{
	m_listener = listener;
	return initWithUrl(url, method);
}

void HTTPRequest::release()
{
	delete this;
}

HTTPRequest::HTTPRequest()
{
	m_listener = 0;
	m_state = kCCHTTPRequestStateIdle;
	m_errorCode = 0;
	m_responseCode = 0;
	m_responseBuffer = NULL;
	m_responseBufferLength = 0;
	m_responseDataLength = 0;
	m_curlState = kCCHTTPRequestCURLStateIdle;
	m_postData = NULL;
	m_postDataLen = 0;
	m_formPost = NULL;
	m_lastPost = NULL;
	m_dltotal = 0;
	m_dlnow = 0;
	m_ultotal = 0;
	m_ulnow = 0;
	m_idSchedule = 0;
}

HTTPRequest::~HTTPRequest(void)
{
	//dmLogInfo("invoke ~HTTPRequest");
	cleanup();
	if (m_listener)
	{
		ext_unregisterLuaCallback(m_listener);
		m_listener = 0;
	}
}

void HTTPRequest::cleanup(void)
{
	//dmLogInfo("invoke cleanup");
	m_state = kCCHTTPRequestStateCleared;
	m_responseBufferLength = 0;
	m_responseDataLength = 0;
	m_postDataLen = 0;
	if (m_postData)
	{
		free(m_postData);
		m_postData = NULL;
	}
	if (m_responseBuffer)
	{
		free(m_responseBuffer);
		m_responseBuffer = NULL;
	}
	if (m_curl)
	{
		curl_easy_cleanup(m_curl);
		m_curl = NULL;
	}
}

bool HTTPRequest::initWithUrl(const char *url, const char* method)
{
	m_curl = curl_easy_init();
	curl_easy_setopt(m_curl, CURLOPT_URL, url);
	curl_easy_setopt(m_curl, CURLOPT_USERAGENT, "libcurl");
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, DEFAULT_TIMEOUT);
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, DEFAULT_TIMEOUT);
	curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L);

	curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYPEER, 0L);
//	curl_easy_setopt(m_curl, CURLOPT_SSL_VERIFYHOST, 0L);//gwj

	if(strcmp(method, "post") == 0)
	{
		curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
		curl_easy_setopt(m_curl, CURLOPT_COPYPOSTFIELDS, "");
	}

	return true;
}

void HTTPRequest::addRequestHeader(const char *header)
{
//	CCAssert(m_state == kCCHTTPRequestStateIdle, "HTTPRequest::addRequestHeader() - request not idle");
//	CCAssert(header, "HTTPRequest::addRequestHeader() - invalid header");
	m_headers.push_back(header);
}

void HTTPRequest::addPOSTValue(const char *key, const char *value)
{
//	CCAssert(m_state == kCCHTTPRequestStateIdle, "HTTPRequest::addPOSTValue() - request not idle");
//	CCAssert(key, "HTTPRequest::addPOSTValue() - invalid key");
	m_postFields[key] = value ? value : "";
}

void HTTPRequest::setPOSTData(const char *data, size_t len)
{
//	CCAssert(m_state == kCCHTTPRequestStateIdle, "HTTPRequest::setPOSTData() - request not idle");
//	CCAssert(data, "HTTPRequest::setPOSTData() - invalid post data");
	m_postFields.clear();
	if (0 == len) {
		len = strlen(data);
	}
	if (0 == len) {
		return;
	}
	if (m_postData)
	{
		free(m_postData);
		m_postDataLen = 0;
		m_postData = NULL;
	}
	m_postData = malloc(len + 1);
	memset(m_postData, 0, len + 1);
	if (NULL == m_postData)
	{
		return;
	}
	memcpy(m_postData, data, len);
	m_postDataLen = len;
	curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
	//curl_easy_setopt(m_curl, CURLOPT_COPYPOSTFIELDS, data);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDS, m_postData);
	curl_easy_setopt(m_curl, CURLOPT_POSTFIELDSIZE, m_postDataLen);
}

void HTTPRequest::setCookieString(const char *cookie)
{
//	CCAssert(m_state == kCCHTTPRequestStateIdle, "HTTPRequest::setAcceptEncoding() - request not idle");
	curl_easy_setopt(m_curl, CURLOPT_COOKIE, cookie ? cookie : "");
}

const std::string HTTPRequest::getCookieString(void)
{
//	CCAssert(m_state == kCCHTTPRequestStateCompleted, "HTTPRequest::getResponseData() - request not completed");
	return m_responseCookies;
}

void HTTPRequest::setTimeout(int timeout)
{
	setConnectTimeout(timeout);
	setReadTimeout(timeout);
}

void HTTPRequest::setConnectTimeout(int timeout)
{
	long to = timeout;
	curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, to);
}

void HTTPRequest::setReadTimeout(int timeout)
{
	long to = timeout;
	curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, to);
}

void HTTPRequest::start()
{
//	CCAssert(m_state == kCCHTTPRequestStateIdle, "HTTPRequest::start() - request not idle");

	m_state = kCCHTTPRequestStateInProgress;
	m_curlState = kCCHTTPRequestCURLStateBusy;
//	retain();

	curl_easy_setopt(m_curl, CURLOPT_HTTP_CONTENT_DECODING, 1L);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeDataCURL);
	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(m_curl, CURLOPT_HEADERFUNCTION, writeHeaderCURL);
	curl_easy_setopt(m_curl, CURLOPT_WRITEHEADER, this);
	curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, false);
	curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, progressCURL);
	curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
	curl_easy_setopt(m_curl, CURLOPT_COOKIEFILE, "");

	//gwj: 用charles抓包的话要加入如下一句
	//详见: http://blog.csdn.net/songtzu/article/details/43971013
#if defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_OSX)
	if(isCharlesRunning())
	{
		curl_easy_setopt(m_curl, CURLOPT_PROXY, "127.0.0.1:8888");
	}
#else
	if(curl_proxy.length() > 0)
	{
		curl_easy_setopt(m_curl, CURLOPT_PROXY, curl_proxy.c_str());
	}
#endif

	m_idSchedule = ext_scheduleUpdate([=](){
		this->update();
	});

#if defined(DM_PLATFORM_WINDOWS)
	CreateThread(NULL,          // default security attributes
		0,             // use default stack size
		requestCURL,   // thread function name
		this,          // argument to thread function
		0,             // use default creation flags
		NULL);
#else
	pthread_t threadt;
	pthread_create(&threadt, NULL, requestCURL, this);
	pthread_detach(threadt);
#endif
}

void HTTPRequest::cancel(void)
{
	if (m_state == kCCHTTPRequestStateIdle || m_state == kCCHTTPRequestStateInProgress)
	{
		m_state = kCCHTTPRequestStateCancelled;
	}
}

int HTTPRequest::getState(void)
{
	return m_state;
}

int HTTPRequest::getResponseStatusCode(void)
{
	//CCAssert(m_state == kCCHTTPRequestStateCompleted, "Request not completed");
	return static_cast<int>(m_responseCode);
}

const std::string HTTPRequest::getResponseHeadersString()
{
	std::string buf;
	for (HTTPRequestHeadersIterator it = m_responseHeaders.begin(); it != m_responseHeaders.end(); ++it)
	{
		buf.append(*it);
	}
	return buf;
}

const std::string HTTPRequest::getResponseString(void)
{
//	CCAssert(m_state == kCCHTTPRequestStateCompleted, "HTTPRequest::getResponseString() - request not completed");
	return m_responseBuffer ? static_cast<char*>(m_responseBuffer) : "";
}

LUA_BUFFER HTTPRequest::getResponseDataLua(void)
{
	//	CCAssert(m_state == kCCHTTPRequestStateCompleted, "HTTPRequest::getResponseDataLua() - request not completed");
	LUA_BUFFER buf;
	buf.ptr = m_responseBuffer;
	buf.size = m_responseDataLength;
	return buf;
}

int HTTPRequest::getResponseDataLength(void)
{
	//CCAssert(m_state == kCCHTTPRequestStateCompleted, "Request not completed");
	return (int)m_responseDataLength;
}

size_t HTTPRequest::saveResponseData(const char *filename)
{
//	CCAssert(m_state == kCCHTTPRequestStateCompleted, "HTTPRequest::saveResponseData() - request not completed");

	FILE *fp = fopen(filename, "wb");
//	CCAssert(fp, "HTTPRequest::saveResponseData() - open file failure");

	size_t writedBytes = m_responseDataLength;
	if (writedBytes > 0)
	{
		fwrite(m_responseBuffer, m_responseDataLength, 1, fp);
	}
	fclose(fp);
	return writedBytes;
}

int HTTPRequest::getErrorCode(void)
{
	return m_errorCode;
}

const std::string HTTPRequest::getErrorMessage(void)
{
	return m_errorMessage;
}

bool HTTPRequest::isCharlesRunning()
{
#if defined(DM_PLATFORM_WINDOWS)
	PROCESSENTRY32W pe;
	HANDLE hSnapShot;
	int bFindCharles = 0;
	int bFindFiddler = 0;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapShot != INVALID_HANDLE_VALUE)
	{
		pe.dwSize = sizeof(PROCESSENTRY32W);
		if(Process32FirstW(hSnapShot, &pe))
		{
			while(1)
			{
				if (StrStrIW(pe.szExeFile, L"Charles.exe"))
				{
					bFindCharles = 1;
					break;
				}
				if(StrStrIW(pe.szExeFile, L"Fiddler.exe"))
				{
					bFindFiddler = 1;
					break;
				}
				if (!Process32NextW(hSnapShot, &pe))
				break;
			}
		}
		CloseHandle(hSnapShot);
	}
	return bFindCharles || bFindFiddler;
#elif defined(DM_PLATFORM_OSX)
	return httpreq_isCharlesRunning() ? true : false;
#else
	return false;
#endif
}

void HTTPRequest::update()
{
	char str[1024];
	if (m_state != kCCHTTPRequestStateCancelled && m_listener)
	{
		sprintf(str, "{\"name\":\"progress\",\"total\":%d,\"dltotal\":%d}", (int)m_dltotal, (int)m_dlnow);
		ext_invokeLuaCallbackWithString(m_listener, str);
	}

	if(m_state != kCCHTTPRequestStateInProgress)
	{
		if (m_listener)
		{
			std::string name;

			switch (m_state)
			{
			case kCCHTTPRequestStateCompleted:
				name = "completed";
				break;
			case kCCHTTPRequestStateCancelled:
				name = "cancelled";
				break;
			case kCCHTTPRequestStateFailed:
				name = "failed";
				break;
			default:
				name = "unknown";
			}
			sprintf(str, "{\"name\":\"%s\"}", name.c_str());
			ext_invokeLuaCallbackWithString(m_listener, str);
		}
		int idSchedule = m_idSchedule;
		m_idSchedule = 0;
		ext_unscheduleUpdate(idSchedule);
		//等到请求线程结束再删除自己
		m_idSchedule = ext_scheduleUpdate([=](){
			//dmLogInfo("curlState=%d", m_curlState);
			if (m_curlState != kCCHTTPRequestCURLStateBusy)
			{
				int idSchedule = m_idSchedule;
				m_idSchedule = 0;
				ext_unscheduleUpdate(idSchedule);
				this->release();
			}
		});
	}
}

void HTTPRequest::onRequest(void)
{
	if (m_postFields.size() > 0)
	{
		curl_easy_setopt(m_curl, CURLOPT_POST, 1L);
		std::stringbuf buf;
		for (Fields::iterator it = m_postFields.begin(); it != m_postFields.end(); ++it)
		{
			char *part = curl_easy_escape(m_curl, it->first.c_str(), 0);
			buf.sputn(part, strlen(part));
			buf.sputc('=');
			curl_free(part);

			part = curl_easy_escape(m_curl, it->second.c_str(), 0);
			buf.sputn(part, strlen(part));
			curl_free(part);

			buf.sputc('&');
		}
		curl_easy_setopt(m_curl, CURLOPT_COPYPOSTFIELDS, buf.str().c_str());
	}

	struct curl_slist *chunk = NULL;
	for (HTTPRequestHeadersIterator it = m_headers.begin(); it != m_headers.end(); ++it)
	{
		chunk = curl_slist_append(chunk, (*it).c_str());
	}

	if (m_formPost)
	{
		curl_easy_setopt(m_curl, CURLOPT_HTTPPOST, m_formPost);
	}

	curl_slist *cookies = NULL;
	curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, chunk);
	//dmLogInfo("gwjgwj,perform request");
	CURLcode code = curl_easy_perform(m_curl);
	curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &m_responseCode);
	curl_easy_getinfo(m_curl, CURLINFO_COOKIELIST, &cookies);

	if (cookies)
	{
		struct curl_slist *nc = cookies;
		std::stringbuf buf;
		while (nc)
		{
			buf.sputn(nc->data, strlen(nc->data));
			buf.sputc('\n');
			nc = nc->next;
		}
		m_responseCookies = buf.str();
		curl_slist_free_all(cookies);
		cookies = NULL;
	}

	if(m_curl)
		curl_easy_cleanup(m_curl);
	m_curl = NULL;
	if (m_formPost)
	{
		curl_formfree(m_formPost);
		m_formPost = NULL;
	}
	curl_slist_free_all(chunk);

	m_errorCode = code;
	m_errorMessage = (code == CURLE_OK) ? "" : curl_easy_strerror(code);
	if(m_state == kCCHTTPRequestStateInProgress)
		m_state = (code == CURLE_OK) ? kCCHTTPRequestStateCompleted : kCCHTTPRequestStateFailed;
	dmLogInfo("gwjgwj,perform request end,code=%d,str=%s", code, m_errorMessage.c_str());
	m_curlState = kCCHTTPRequestCURLStateClosed;
}

size_t HTTPRequest::onWriteData(void *buffer, size_t bytes)
{
	if (m_responseDataLength + bytes + 1 > m_responseBufferLength)
	{
		m_responseBufferLength += BUFFER_CHUNK_SIZE;
		m_responseBuffer = realloc(m_responseBuffer, m_responseBufferLength);
	}

	memcpy(static_cast<char*>(m_responseBuffer) + m_responseDataLength, buffer, bytes);
	m_responseDataLength += bytes;
	static_cast<char*>(m_responseBuffer)[m_responseDataLength] = 0;
	return bytes;
}

size_t HTTPRequest::onWriteHeader(void *buffer, size_t bytes)
{
	char *headerBuffer = new char[bytes + 1];
	headerBuffer[bytes] = 0;
	memcpy(headerBuffer, buffer, bytes);
	m_responseHeaders.push_back(headerBuffer);
	delete []headerBuffer;
	return bytes;
}

int HTTPRequest::onProgress(double dltotal, double dlnow, double ultotal, double ulnow)
{
	m_dltotal = dltotal;
	m_dlnow = dlnow;
	m_ultotal = ultotal;
	m_ulnow = ulnow;

	return m_state == kCCHTTPRequestStateCancelled ? 1: 0;
}

#ifdef DM_PLATFORM_WINDOWS
DWORD WINAPI HTTPRequest::requestCURL(LPVOID userdata)
{
	static_cast<HTTPRequest*>(userdata)->onRequest();
	return 0;
}
#else // DM_PLATFORM_WINDOWS
void *HTTPRequest::requestCURL(void *userdata)
{
	static_cast<HTTPRequest*>(userdata)->onRequest();
	return NULL;
}
#endif // DM_PLATFORM_WINDOWS

size_t HTTPRequest::writeDataCURL(void *buffer, size_t size, size_t nmemb, void *userdata)
{
	return static_cast<HTTPRequest*>(userdata)->onWriteData(buffer, size *nmemb);
}

size_t HTTPRequest::writeHeaderCURL(void *buffer, size_t size, size_t nmemb, void *userdata)
{
	return static_cast<HTTPRequest*>(userdata)->onWriteHeader(buffer, size *nmemb);
}

int HTTPRequest::progressCURL(void *userdata, double dltotal, double dlnow, double ultotal, double ulnow)
{
	return static_cast<HTTPRequest*>(userdata)->onProgress(dltotal, dlnow, ultotal, ulnow);
}
