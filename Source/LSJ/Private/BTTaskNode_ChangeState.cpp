// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChangeState.h"
#include "AICharacter.h"
#include "AIController.h"
#include "AIStateBackDash.h"
#include "AIStateRun.h"
#include "AIStateWalkBack.h"
#include "AIStateWalkForward.h"
#include "AIStateIdle.h"
UBTTaskNode_ChangeState::UBTTaskNode_ChangeState ( )
{
	
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_ChangeState::ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory )
{
	EBTNodeResult::Type Result = Super::ExecuteTask ( OwnerComp , NodeMemory );
	AAIController* aiController = OwnerComp.GetAIOwner ( );
	if ( aiController )
	{
		AAICharacter* Enemy = Cast<AAICharacter> ( aiController->GetPawn ( ) );
		if ( Enemy )
		{
			UActorComponent* stateComponent = nullptr;

			// 적절한 상태 클래스에 따라 상태 변경
			  // 적절한 상태 클래스에 따라 상태 변경
			if ( newStateClass == UAIStateWalkBack::StaticClass ( ) )
			{
				stateComponent = Enemy->GetAIStateWalkBack ( );
			}
			else if ( newStateClass == UAIStateWalkForward::StaticClass ( ) )
			{
				stateComponent = Enemy->GetAIStateWalkForward ( );
			}

			if ( stateComponent )
			{
				Enemy->ChangeState ( Cast<IAIStateInterface> ( stateComponent ) );

				// 상태 완료시 호출될 델리게이트 바인딩
				if ( UAIStateWalkBack* stateWalkBack = Cast<UAIStateWalkBack> ( stateComponent ) )
				{
					stateWalkBack->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
					/*				FTimerHandle handle;
									GetWorld ( )->GetTimerManager ( ).SetTimer ( handle , FTimerDelegate::CreateLambda ( [stateWalkBack]( ) {
										stateWalkBack->Exit ( );
										} ) , 1.0f , false );*/
				}
				else if ( UAIStateWalkForward* stateWalkForward = Cast<UAIStateWalkForward> ( stateComponent ) )
				{
					stateWalkForward->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
					/*		FTimerHandle handle;
							GetWorld ( )->GetTimerManager ( ).SetTimer ( handle , FTimerDelegate::CreateLambda ( [stateWalkForward]( ) {
								stateWalkForward->Exit ( );
								} ) , 1.0f , false );*/
				}

				bIsWaitingForState = true;
				cachedOwnerComp = &OwnerComp;

				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTaskNode_ChangeState::TickTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds )
{
	Super::TickTask ( OwnerComp , NodeMemory , DeltaSeconds );

	// 상태 완료 대기 중일 때는 Tick을 계속 호출
	if ( !bIsWaitingForState )
	{
		FinishLatentTask ( OwnerComp , EBTNodeResult::Succeeded );
	}
}

void UBTTaskNode_ChangeState::OnStateCompleted ( )
{
	bIsWaitingForState = false;

	if ( cachedOwnerComp )
	{
		FinishLatentTask ( *cachedOwnerComp , EBTNodeResult::Succeeded );
	}
}
