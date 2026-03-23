// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LuaSubsystem.generated.h"

struct lua_State;
/**
 * 
 */
UCLASS()
class MYPROJECT_API ULuaSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	bool DoFile(const FString& FilePath);
	bool CallFunction(const FString& FunctionName, const TArray<FString>& Args);

	void RegisterFunctions();

private:
	lua_State* LuaState;
};
