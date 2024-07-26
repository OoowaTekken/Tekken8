// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "CPP_InputControl.generated.h"

UCLASS()
class KYOULEE_API ACPP_InputControl : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACPP_InputControl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* uIMC_Player;
	
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_CharaterMove;

	UFUNCTION()
	void OnMyActionMove(const FInputActionValue& ActionValue);

};
