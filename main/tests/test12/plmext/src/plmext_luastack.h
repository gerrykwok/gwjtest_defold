#include <dmsdk/sdk.h>

class LuaStack
{
public:
	static LuaStack* getInstance();
	bool initWithLuaState(lua_State *L);

	/**
	@brief Method used to get a pointer to the lua_State that the script module is attached to.
	@return A pointer to the lua_State that the script module is attached to.
	*/
	lua_State* getLuaState(void) {
		return _state;
	}

	/**
	@brief Remove Lua function reference
	*/
	virtual void removeScriptHandler(int nHandler);
	/**
	@brief Execute a scripted global function.
	@brief The function should not take any parameters and should return an integer.
	@param functionName String object holding the name of the function, in the global script environment, that is to be executed.
	@return The integer value returned from the script function.
	*/
	virtual int executeGlobalFunction(const char* functionName);
	virtual void clean(void);
	virtual void pushInt(int intValue);
	virtual void pushFloat(float floatValue);
	virtual void pushLong(long longValue);
	virtual void pushBoolean(bool boolValue);
	virtual void pushString(const char* stringValue);
	virtual void pushString(const char* stringValue, int length);
	virtual void pushNil(void);
//	virtual void pushObject(Ref* objectValue, const char* typeName);
//	virtual void pushLuaValue(const LuaValue& value);
//	virtual void pushLuaValueDict(const LuaValueDict& dict);
//	virtual void pushLuaValueArray(const LuaValueArray& array);
	virtual bool pushFunctionByHandler(int nHandler);

	virtual int executeFunction(int numArgs);
	virtual int executeFunctionByHandler(int nHandler, int numArgs);

public:
	LuaStack(void)
	: _state(NULL)
	, _callFromLua(0)
	{
	}
protected:
	lua_State *_state;
	int _callFromLua;
};