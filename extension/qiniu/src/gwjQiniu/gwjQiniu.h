//////////////////////////////////////////////////////////////////////////
class gwjQiniu
{
public:
	gwjQiniu();
	static gwjQiniu* startNewUpload(const char *token, const char *localPath, const char *name, int luaCallback);
protected:
	void init(const char *token, const char *localPath, const char *name, int luaCallback);
	void startUpload();
	void onUploadEnd(int code, const char *message);
private:
	std::string m_token;
	std::string m_localPath;
	std::string m_name;
	int m_luaCallback;

	unsigned int m_idSchedule;
};
