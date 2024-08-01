// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Character.h"
#include "Tekken8CharacterParent.h"
#include "AICharacter.generated.h"

UCLASS()
class LSJ_API AAICharacter : public ATekken8CharacterParent
{
	GENERATED_BODY()

	UPROPERTY (EditDefaultsOnly)
	class UAICharacterAnimInstance * animInstance;

	ECharacterState eCurrentState;
	class IAIStateInterface* currentState;

	UPROPERTY ()
	class UAIStateBackDash* stateBackDash;
	UPROPERTY ( )
	class UAIStateRun* stateRun;
	UPROPERTY ( )
	class UAIStateWalkBack* stateWalkBack;
	UPROPERTY ( )
	class UAIStateWalkForward* stateWalkForward;
	UPROPERTY ( )
	class UAIStateAttackLF* stateAttackLF;
	UPROPERTY ( )
	class UAIStateAttackRH* stateAttackRH;
	UPROPERTY ( )
	class UAIStateIdle* stateIdle;
	bool IsPlayer1;
public:
	// Sets default values for this character's properties
	AAICharacter();

	// 상태 공격 객체에 대한 접근 메서드 추가
	class UAIStateAttackLF* GetAIStateAttackLF ( ) const { return stateAttackLF; }
	void StateAttackLF ( class UAIStateAttackLF* val ) { stateAttackLF = val; }
	class UAIStateAttackRH* GetAIStateAttackRH ( ) const { return stateAttackRH; }
	void StateAttackRH ( class UAIStateAttackRH* val ) { stateAttackRH = val; }

	//공격 콜리전
	UPROPERTY ( )
	class USphereComponent* collisionLH;
	UPROPERTY ( )
	class USphereComponent* collisionRH;
	UPROPERTY ( )
	class USphereComponent* collisionLF;
	UPROPERTY ( )
	class USphereComponent* collisionRF;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ChangeState (class IAIStateInterface* NewState );
	void UpdateState ( const float& deltatime );
	void ExitCurrentState ();

	// 상태 이동 객체에 대한 접근 메서드 추가
	UAIStateWalkForward* GetAIStateWalkForward ( ) const { return stateWalkForward; }
	UAIStateWalkBack* GetAIStateWalkBack ( ) const { return stateWalkBack; }
	UAIStateRun* GetAIStateRun ( ) const { return stateRun; }
	UAIStateBackDash* GetAIStateBackDash ( ) const { return stateBackDash; }

};
