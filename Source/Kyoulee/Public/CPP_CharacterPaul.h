// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_Tekken8CharacterParent.h"
#include "PlayerCameraPawn.h"
#include "CPP_CharacterPaul.generated.h"


// 델리게이트
DECLARE_DELEGATE(FActoin);


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
	bool bLeftPunch : 1;
	bool bRightPunch : 1;
	bool bLeftKick : 1;
	bool bRightKick : 1;
};
/**
 * @Title 키에대한 입력처리를 위한 스트럭트입니다.
 */
USTRUCT(Atomic, BlueprintType)
struct FFrameStatus
{
	GENERATED_BODY()
public:

	// 사용중인 프레임 딜레이입니다.
	// 0일시 기본으로 동작이 초기화 됩니다.
	int32 FrameUsing = 0;
	// 막힌 프레임 정보입니다.
	// 이프레임 동안은 동작을 할 수 없습니다.
	int32 FrameBlockUsing = 0;
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
	FActoin action;
 };

UCLASS()
class KYOULEE_API ACPP_CharacterPaul : public ACPP_Tekken8CharacterParent
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
	int32 GetCurrInputKeyValue();

	UFUNCTION ( )
	FInputKey GetCurrInputKey ( );

	UFUNCTION()
	bool SetCurrInputKeyValue(int32 value);
	
	UFUNCTION()
	bool SetCurrInputKey( FInputKey value);

	UFUNCTION()
	int32 InputKeyValue(int ArrowKey, bool LeftArm, bool RightArm, bool LeftKick, bool RightKick);

	int32 animationEnd = true;
	
	/**
	 * @title Command Tree
	 * @brief 커멘트 트리에 대한 정보 및 세팅입니다.
	 */
	TMap<int32, FCommandTree*> mBaseCommandTree;
	TMap<int32 , FCommandTree*> mCurrCommandTree;
	FCommandTree* sCurrCommand;
	int32 CountIdleFrame = 0;
	int32 CountStarFrame = 0;
	int32 currKeyValue = 0;

	void SettingCommandTree();

	FCommandTree *CreateCommandTree(int32 timingStart, int32 timingEnd, int32 timingAction, void (ACPP_CharacterPaul::*fptr)());
	
	FCommandTree* AddCommandTree(TMap<int32, FCommandTree*>& CurrCommandTree, int32 keyValue, int32 timingStart, int32 timingEnd, int32 timingAction, void(ACPP_CharacterPaul::* fptr)());

	FCommandTree* AddCommandBaseTree ( TArray<int> arrayTreeCommand , int32 keyValue , int32 timingStart , int32 timingEnd , int32 timingAction , void(ACPP_CharacterPaul::* fptr)() );
	void SetSelfReLinkTree ( TArray<int32> arrayTreeCommand );

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool DebugingMode = 0;

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
	FFrameStatus sFrameStatus;
	float fFrameTime = (1 / 60)  * 1000 ;
	float fCurrTimeForFrame = 60;
	int32 iCurrFrame = 0;
	
	void skillfinctiontest() {};
	
	float fUpVector;

	/**
	 * @title Owner State
	 * @brief 상태 정보입니다.
	 */
	class USkeletalMeshComponent* uCharacterMesh;

	eHitDecision DefencePoint;


	/**
	 * @title Target Point
	 * @brief 상대에 대한 정보
	 */
	 UPROPERTY(EditDefaultsOnly)
	 ACharacter *aOpponentPlayer;

	/**
	 * @title Next Location Data 처리
	 * @brief 애니메이션을 위한 작업입니다.
	 */
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* uWalkMontage;
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* uRunMontage;
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* uJumpMontage;

	
	class UAnimMontage* uCurrMotage;
	FVector ToLocation;
	
	void SetToLocationPoint(float x, float y, float z);
	void SetToLocationPoint ( FVector vector );
	void SetToWorldLocationPoint ( FVector vector );

	void AnimationFrame ();

	FVector RelativePointVector ( float x , float y , float z );


	/**
	 * @title Camera Effect
	 * @brief 히트의 충돌체 카메라 효과를 위한 작업입니다.
	 */
	AGameMode_MH* GameModeMH;
	// APlayerCameraPawn* aMainCamera;

	/**
	* @title 메인트리
	* @brief 시작 지점 트리를 가지고 있습니다.
	*	모든 스킬트리는 맵형식의 반복된 값을 가지고 있음으로 참고 바랍니다.
 	*/
	void CommandIdle ( );
	void CommandStar ( );
	void CommandMoveForward ( );
	void CommandMoveForwarDash();
	void CommandMoveBack ();
	void CommandMoveBackDash();
	void CommandJump ( );
	void CommandMoveLateralUp ( );
	void CommandDownCrouch ( );
	void CommandMoveLateralDown( );
	void CommandLeadJab ( );
	void CommandCrossStaight ( );
	void CommandJingun ( );
	void CommandHighKick ();
	bool CommandAllStop ( );
// 
// 	virtual int32 GetCurrentHp ( ) const override {return 0;};
// 	virtual bool HitDecision ( FSkellInfo hitPosition , ATekken8CharacterParent* hitActorInterface )  override;

/**
 * @title : Interaction
 *
 */
	virtual bool HitDecision ( FAttackInfoInteraction attackInfo , ACPP_Tekken8CharacterParent* ownerHitPlayer ) override;
};
	            
