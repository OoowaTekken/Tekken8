// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

AAICharacterController::AAICharacterController ( )
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject ( TEXT ( "/Script/AIModule.BehaviorTree'/Game/LSJ/AI/BT_AI.BT_AI'" ) );
	if ( BTObject.Succeeded ( ) )
	{
		behaviortree = BTObject.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject ( TEXT ( "/Script/AIModule.BlackboardData'/Game/LSJ/AI/BB_AI.BB_AI'" ) );
	if ( BBObject.Succeeded ( ) )
	{
		blackboardData = BBObject.Object;
	}
}

void AAICharacterController::OnPossess ( APawn* InPawn )
{
	if ( !RunBehaviorTree ( behaviortree ) )
	{
		UE_LOG ( LogTemp , Error , TEXT ( "AIController couldn't run behavior tree!" ) );
	}
}

