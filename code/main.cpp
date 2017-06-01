// C++调用Lua
#include <iostream>
#include <string.h>
using namespace std;

//	加载Lua53.lib
#pragma comment(lib, "..\\lib\\Lua53.lib")

extern "C"
{
#include "Lua/lua.h"
#include "Lua/lauxlib.h"
#include "Lua/lualib.h"
};


lua_State* L;

int C2Lua_Add( int nX, int nY )
{
	int nSum;
	lua_getglobal( L, "C2Lua_Add" );
	lua_pushnumber( L, nX );
	lua_pushnumber( L, nY );
	int nresult = lua_pcall( L, 2, 1, 0 );
	if ( nresult )
	{
		const char *pErrorMsg = lua_tostring(L, -1);
		cout << pErrorMsg << endl;  
		lua_close(L);  
		return -1;  
	}
	nSum = (int)lua_tonumber( L, -1 );
	lua_pop( L, 1 );
	return nSum;
}


void main( )
{
	int nSum = 0;
	
	L = luaL_newstate( );
	luaopen_base( L );
	luaL_openlibs( L );
	//	加载lua文件
	int nRet = luaL_loadfile(L, "..\\code\\C2Lua.lua");
	if ( nRet )
	{
		cout << " load file error " << endl;
		return ;
	}

	//	运行lua文件
	nRet = lua_pcall( L, 0, 0, 0);
	if ( nRet )
	{
		cout << "pcall error." << endl;
		return;
	}

	//	调用函数
	int nX = 5;
	int nY = 8;
	nSum = C2Lua_Add( nX, nY );
	cout << nX << " + " << nY << " = " << nSum << endl;

	//	读取变量
	lua_getglobal( L, "teststr");
	string str = lua_tostring( L, -1 );
	cout << "teststr is : " << str.c_str() << endl;
	lua_pop( L, 1 );

	////	读取table
	lua_getglobal( L, "tabletest" );
	lua_getfield( L, -1, "id" );
	str = lua_tostring( L, -1 );
	cout << "tabletest id is : " << str.c_str() << endl;
	lua_pop( L, 1 );

	lua_getfield( L, -1, "num" );
	str = lua_tostring( L, -1 );
	cout << "tabletest num is : " << str.c_str() << endl;


	system("pause");
	return;
}