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
			if ( newStateClass == UAIStateWalkBack::StaticClass())
			{
				stateComponent = Enemy->GetAIStateWalkBack();
			}
			else if ( newStateClass == UAIStateWalkForward::StaticClass())
			{
				stateComponent = Enemy->GetAIStateWalkForward();
			}
			else if ( newStateClass == UAIStateBackDash::StaticClass())
			{
				stateComponent = Enemy->GetAIStateBackDash();
			}

			if ( stateComponent )
			{
				// 상태 완료시 호출될 델리게이트 바인딩
				if ( UAIStateWalkBack* stateWalkBack = Cast<UAIStateWalkBack> ( stateComponent ) )
				{
					stateWalkBack->SetDistance(distance);
					if ( !stateWalkBack->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
					stateWalkBack->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateWalkForward* stateWalkForward = Cast<UAIStateWalkForward> ( stateComponent ) )
				{
					stateWalkForward->SetDistance ( distance );
					if ( !stateWalkForward->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
					stateWalkForward->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}
				else if ( UAIStateBackDash* stateBackDash = Cast<UAIStateBackDash> ( stateComponent ) )
				{
					if ( !stateBackDash->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeState::OnStateCompleted ) )
						stateBackDash->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeState::OnStateCompleted );
				}

				bIsWaitingForState = true;
				cachedOwnerComp = &OwnerComp;
				Enemy->ChangeState ( Cast<IAIStateInterface> ( stateComponent ) );
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
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
}
