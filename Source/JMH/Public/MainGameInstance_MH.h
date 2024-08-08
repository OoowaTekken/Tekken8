// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance_MH.generated.h"

/**
 * 
 */
UCLASS()
class JMH_API UMainGameInstance_MH : public UGameInstance
{
	GENERATED_BODY()

public:
	// 변수 선언
	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	int32 CharacterSelectionIndex; //1.paul,2.Kazuya

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	int32 GameModeSelectionIndex; //1.PvsP,2.PvsCPU

	UPROPERTY(BlueprintReadWrite, Category = "Game Data")
	bool IsPlayerOnLeftSide;//true.Left,false.Right 
	
};
