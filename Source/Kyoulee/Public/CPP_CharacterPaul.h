// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_CharacterPaul.generated.h"


USTRUCT(Atomic, BlueprintType)
struct FInputKey
{
	GENERATED_BODY()
public:
	//방향키
	// 아래 왼쪽
	bool bKey1 : 1;
	// 아래
	bool bKey2 : 1;
	// 아래 오른쪽
	bool bKey3 : 1;
	// 중간 왼쪽
	bool bKey4 : 1;
	// 중간 오른쪽
	bool bKey6 : 1;
	// 위 왼쪽
	bool bKey7 : 1;
	// 위
	bool bKey8 : 1;
	// 위 오른쪽
	bool bKey9 : 1;

	// 왼쪽
	bool bLeftArm : 1;
	bool bRightArm : 1;
	bool bLeftKick : 1;
	bool bRightKick : 1;
};

/**
 * @Title 키에대한 입력처리를 위한 스트럭트입니다.
 */
USTRUCT(Atomic, BlueprintType)
struct FCommandTree
{
	GENERATED_BODY()
public:

	// 이전 기술 목록
	TArray<int32> PrevTrees;
	// 다음 가능기술 목록
	TMap<int32, FCommandTree*> NextTrees;
	// 시작시간
	int32 timingStart;
	// 끝시간
	int32 timingEnd;
	// 발동 시간
	int32 timingAction;
	
	// 발동 함수
	void (* ActionFunction)();
 };
 

UCLASS()
class KYOULEE_API ACPP_CharacterPaul : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_CharacterPaul();


	/**
	 * @title 기본적인 키에대한 정보입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInputKey CurrInputKey;
	
	UFUNCTION()
	int32 GetCurrInputKey();

	UFUNCTION()
	bool SetCurrInputKey(int32 value);
	
	UFUNCTION()
	int32 ShowInputKey(int ArrowKey, bool LeftArm, bool RightArm, bool LeftKick, bool RightKick);

	/**
	 * @title Command Tree
	 * @brief 커멘트 트리에 대한 정보 및 세팅입니다.
	 */
	TMap<int32, FCommandTree*> mBaseCommandTree;

	void SettingCommandTree();

	FCommandTree *CreatCommandTree(int32 timingStart, int32 timingEnd, int32 timingAction, void (*fptr)());
	
	FCommandTree *AddCommand(int32 keyValue);
	void skillfinctiontest(){};
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	 * @Title 프레임 시스템을 만듬니다.
	 * @brief 1초당 60프레임으로 실행 되는 시스템입니다.
	 */
	void FrameSystem();

	float fFrameTime = 1 / 60;
	float fCurrTimeForFrame = 0;


	/**
	* @title 메인트리
	* @brief 시작 지점 트리를 가지고 있습니다.
	*	모든 스킬트리는 맵형식의 반복된 값을 가지고 있음으로 참고 바랍니다.
 	*/
	//TMap<int32, FCommandTree> mMainCommandTree;
};
	            