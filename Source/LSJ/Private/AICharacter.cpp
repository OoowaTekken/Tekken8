// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AICharacterController.h"
#include "AICharacterAnimInstance.h"
#include "AIStateBackDash.h"
#include "AIStateRun.h"
#include "AIStateWalkBack.h"
#include "AIStateWalkForward.h"
#include "AIStateIdle.h"
// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> meshFinder(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'" ) );
	if ( meshFinder.Succeeded ( ) )
	{
		GetMesh ( )->SetSkeletalMesh (meshFinder.Object );
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> animFinder ( TEXT ( "/Script/Engine.AnimBlueprint'/Game/LSJ/Blueprint/AB_AICharacter.AB_AICharacter_C'" ) );
	if ( animFinder.Succeeded ( ) )
	{
		GetMesh ( )->SetAnimInstanceClass ( animFinder.Class );
	}
	GetMesh( )->SetRelativeLocation(FVector(0,0,-80.f));
	GetMesh ( )->SetRelativeRotation ( FRotator (  0, -90.f , 0 ) );
	stateBackDash = CreateDefaultSubobject<UAIStateBackDash>(TEXT("stateBackDash"));
	stateBackDash->SetStateOwner(this);
	stateRun = CreateDefaultSubobject<UAIStateRun> ( TEXT ( "stateRun" ) );
	stateRun->SetStateOwner ( this );
	stateWalkBack = CreateDefaultSubobject<UAIStateWalkBack> ( TEXT ( "stateWalkBack" ) );
	stateWalkBack->SetStateOwner ( this );
	stateWalkForward = CreateDefaultSubobject<UAIStateWalkForward> ( TEXT ( "stateWalkForward" ) );
	stateWalkForward->SetStateOwner ( this );
	stateIdle = CreateDefaultSubobject<UAIStateIdle> ( TEXT ( "stateIdle" ) );
	stateIdle->SetStateOwner ( this );
	AIControllerClass = AAICharacterController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	stateWalkForward->SetComponentTickEnabled ( false );
	stateWalkBack->SetComponentTickEnabled ( false );
	animInstance = Cast<UAICharacterAnimInstance> (GetMesh()->GetAnimInstance());
	if ( animInstance )
	{
		animInstance->OnMontageEnded.AddDynamic ( this , &AAICharacter::HandleOnMontageEnded );
	}
	currentState = stateIdle;

	//FTimerHandle handle;
	//GetWorld ( )->GetTimerManager ( ).SetTimer ( handle ,FTimerDelegate::CreateLambda ([this]() {
	//	ChangeState ( stateWalkBack );
	//	}) , 10.0f ,false);
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UpdateState( );
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAICharacter::ChangeState ( IAIStateInterface* NewState )
{
	//hit 했을때 필요할거 같다.
	//if ( currentState) {
	//	currentState->Exit ( );
	//}
	currentState = NewState;

	if ( currentState ) {
		currentState->Enter (animInstance);
	}
}

void AAICharacter::UpdateState()
{
	if ( currentState )
		currentState->Execute ( );
}

void AAICharacter::HandleOnMontageEnded ( UAnimMontage* Montage , bool bInterrupted )
{
	//currentState->Exit();
	// Montage가 끝났을 때의 처리 로직
	if ( bInterrupted )
	{
		// Animation Montage가 정상적으로 끝나지 않고 중간에 인터럽트되었습니다. 인터럽트는 다른 애니메이션이 재생되었거나, 명시적으로 중단되는 등의 이유로 발생할 수 있습니다.
		UE_LOG ( LogTemp , Warning , TEXT ( "Montage was interrupted. %s" ) , *Montage->GetName ( ) );
	}
	else
	{
		// Animation Montage가 정상적으로 끝났습니다.
		UE_LOG ( LogTemp , Warning , TEXT ( "Montage ended successfully. %s" ) , *Montage->GetName ( ) );
	}
}