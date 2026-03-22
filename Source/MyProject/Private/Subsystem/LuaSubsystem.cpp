// Fill out your copyright notice in the Description page of Project Settings.

extern "C" {
}

#include "Subsystem/LuaSubsystem.h"

void ULuaSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void ULuaSubsystem::Deinitialize()
{
}

bool ULuaSubsystem::DoFile(const FString& FilePath)
{
	return false;
}

bool ULuaSubsystem::CallFunction(const FString& FunctionName, const TArray<FString>& Args)
{
	return false;
}
