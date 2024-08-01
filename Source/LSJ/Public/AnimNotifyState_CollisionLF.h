// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_CollisionLF.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UAnimNotifyState_CollisionLF : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UE_DEPRECATED ( 5.0 , "This function is deprecated. Use the other NotifyBegin instead." )
		virtual void NotifyBegin ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float TotalDuration );
	//UE_DEPRECATED ( 5.0 , "This function is deprecated. Use the other NotifyTick instead." )
	//	virtual void NotifyTick ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation , float FrameDeltaTime );
	UE_DEPRECATED ( 5.0 , "This function is deprecated. Use the other NotifyEnd instead." )
		virtual void NotifyEnd ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation );
};
