// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/LuaSubsystem.h"
#include <string>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

void ULuaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	LuaState = luaL_newstate();
	luaL_openlibs(LuaState);

	RegisterFunctions();

}

void ULuaSubsystem::Deinitialize()
{
	if (LuaState)
	{
		lua_close(LuaState);
		LuaState = nullptr;
	}

	Super::Deinitialize();
}

void ULuaSubsystem::RegisterFunctions()
{
	// Example: Register a simple print function
	lua_pushcfunction(LuaState, [](lua_State* L) -> int {
		const char* Message = lua_tostring(L, 1);
		UE_LOG(LogTemp, Log, TEXT("Lua: %s"), UTF8_TO_TCHAR(Message));
		return 0; // Number of return values
	});
	lua_setglobal(LuaState, "Print");
}

bool ULuaSubsystem::DoFile(const FString& FilePath)
{
	std::string Path = TCHAR_TO_UTF8(*FilePath);

	if (luaL_dofile(LuaState, Path.c_str()) != LUA_OK)
	{
		const char* ErrorMessage = lua_tostring(LuaState, -1);
		UE_LOG(LogTemp, Error, TEXT("Failed to execute Lua file: %s"), UTF8_TO_TCHAR(ErrorMessage));
		lua_pop(LuaState, 1); // Remove error message from stack
		return false;
	}

	return true;
}

bool ULuaSubsystem::CallFunction(const FString& FunctionName, const TArray<FString>& Args)
{
	lua_getglobal(LuaState, TCHAR_TO_UTF8(*FunctionName));

	if (!lua_iscfunction(LuaState, -1))
	{
		UE_LOG(LogTemp, Error, TEXT("Lua function '%s' not found"), *FunctionName);
		lua_pop(LuaState, 1); // Remove non-function from stack
		return false;
	}

	if(lua_pcall(LuaState,0,0,0) != LUA_OK)
	{
		const char* ErrorMessage = lua_tostring(LuaState, -1);
		UE_LOG(LogTemp, Error, TEXT("Failed to call Lua function: %s"), UTF8_TO_TCHAR(ErrorMessage));
		lua_pop(LuaState, 1); // Remove error message from stack
		return false;
	}

	return true;
}
