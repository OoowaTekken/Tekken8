// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Distance.h"
#include "AICharacterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

UBTDecorator_Distance::UBTDecorator_Distance()
{
	NodeName = TEXT ( "DistancePlayer" );
}


bool UBTDecorator_Distance::CalculateRawConditionValue ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory ) const
{
	bool bResult = Super::CalculateRawConditionValue ( OwnerComp , NodeMemory );

	APawn* ControllingPawn = OwnerComp.GetAIOwner ( )->GetPawn ( );
	if (nullptr == ControllingPawn)
		return false;
	
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject( FName(TEXT("Player"))));
	if ( nullptr == Target )
		return false;
	/*TArray<AActor*> outActors;
	UGameplayStatics::GetAllActorsOfClass ( GetWorld ( ) , ACharacter::StaticClass ( ) , outActors );*/
	//UE_LOG(LogTemp,Error,TEXT("%f" ), ControllingPawn->GetDistanceTo ( Target ) );
	bResult = (ControllingPawn->GetDistanceTo(Target) <= distance);
	return bResult;
}
