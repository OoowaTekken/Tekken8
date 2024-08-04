// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_ChangeAttack.h"
#include "AICharacter.h"
#include "AIController.h"
#include "AIStateAttackLF.h"
#include "AIStateAttackRH.h"
#include "AIStateComboLaserAttack.h"
UBTTaskNode_ChangeAttack::UBTTaskNode_ChangeAttack ( )
{

	bNotifyTick = true;
}

EBTNodeResult::Type UBTTaskNode_ChangeAttack::ExecuteTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory )
{
	EBTNodeResult::Type Result = Super::ExecuteTask ( OwnerComp , NodeMemory );
	AAIController* aiController = OwnerComp.GetAIOwner ( );
	if ( aiController )
	{
		AAICharacter* Enemy = Cast<AAICharacter> ( aiController->GetPawn ( ) );
		if ( Enemy )
		{
			UAIStateComponent* stateComponent = nullptr;

			// 적절한 상태 클래스에 따라 상태 변경
			if ( newStateClass == UAIStateAttackLF::StaticClass ( ) )
			{
				stateComponent = Enemy->GetAIStateAttackLF ( );
			}
			else if ( newStateClass == UAIStateAttackRH::StaticClass ( ) )
			{
				stateComponent = Enemy->GetAIStateAttackRH ( );
			}
			else if ( newStateClass == UAIStateComboLaserAttack::StaticClass ( ) )
			{
				stateComponent = Enemy->StateComboLaserAttack ( );
			}

			if ( stateComponent )
			{
				// 상태 완료시 호출될 델리게이트 바인딩
				if ( UAIStateAttackLF* stateAttackLF = Cast<UAIStateAttackLF> ( stateComponent ) )
				{
					if ( !stateAttackLF->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted ) )
						stateAttackLF->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted );
				}
				else if ( UAIStateAttackRH* stateAttackRH = Cast<UAIStateAttackRH> ( stateComponent ) )
				{
					if ( !stateAttackRH->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted ) )
						stateAttackRH->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted );
				}
				else if ( UAIStateComboLaserAttack* stateComboLaserAttack = Cast<UAIStateComboLaserAttack> ( stateComponent ) )
				{
					if ( !stateComboLaserAttack->OnStateComplete.IsAlreadyBound ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted ) )
						stateComboLaserAttack->OnStateComplete.AddDynamic ( this , &UBTTaskNode_ChangeAttack::OnStateCompleted );
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

void UBTTaskNode_ChangeAttack::TickTask ( UBehaviorTreeComponent& OwnerComp , uint8* NodeMemory , float DeltaSeconds )
{
	Super::TickTask ( OwnerComp , NodeMemory , DeltaSeconds );

	// 상태 완료 대기 중일 때는 Tick을 계속 호출
	if ( !bIsWaitingForState )
	{
		FinishLatentTask ( OwnerComp , EBTNodeResult::Succeeded );
	}
}

void UBTTaskNode_ChangeAttack::OnStateCompleted ( )
{
	bIsWaitingForState = false;
}

