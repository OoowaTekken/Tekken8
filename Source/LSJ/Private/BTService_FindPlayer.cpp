// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindPlayer.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "CPP_CharacterPaul.h"

UBTService_FindPlayer::UBTService_FindPlayer ( )
{
	NodeName = TEXT ( "FindPlayer" );
	Interval = 0.5f;
}

void UBTService_FindPlayer::TickNode ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds )
{
	OwnerComp.GetBlackboardComponent ( )->SetValueAsObject ( FName("Player" ) , UGameplayStatics::GetActorOfClass ( GetWorld ( ) , ACPP_CharacterPaul::StaticClass ( ) ) );
}
