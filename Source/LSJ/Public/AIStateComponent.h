// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIStateInterface.h"
#include "AIStateComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FOnStateWalkForwardComplete );
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LSJ_API UAIStateComponent : public UActorComponent , public IAIStateInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	class ACharacter* player;
	class ACharacter* owner;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Enter (class UAICharacterAnimInstance* pAnimInstance );
	virtual void Execute ( const float& deltatime );
	virtual void Exit ( );
	virtual void SetStateOwner (class ACharacter* pOwner);

		UPROPERTY(BlueprintAssignable)
    FOnStateWalkForwardComplete OnStateComplete;
};
