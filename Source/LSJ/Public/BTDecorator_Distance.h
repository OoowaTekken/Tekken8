// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_Distance.generated.h"

/**
 * 
 */
UCLASS()
class LSJ_API UBTDecorator_Distance : public UBTDecorator
{
	GENERATED_BODY()
	//attack range < walk range < run range
	UPROPERTY(EditAnywhere,Category="Distance",meta=(AllowPrivateAccess="true") )
	float distance;
public:
	UBTDecorator_Distance ( );

protected:
	virtual bool CalculateRawConditionValue ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory ) const override;
};
