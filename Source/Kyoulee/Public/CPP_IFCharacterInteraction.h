// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CPP_IFCharacterInteraction.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCPP_IFCharacterInteraction : public UInterface
{
	GENERATED_BODY()
};



enum eHitDecision
{
	Top,
	Middle,
	Lower,
	Special,
	Grap,
	GuardBreak,
	Other
};


/**
 * @Title 캐릭터의 상호작용을 위한 InterFace입니다.
 * @brie 히트에 대한 처리를 준비하고 있습니다. 
 */
class KYOULEE_API ICPP_IFCharacterInteraction
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * @brie 현재 체력을 확인하기위한 함수
	 */
	virtual float GetCurrentHp() const = 0;

	/**
	 * @func HitDecision
	 * @param (eHitDecision hitPosition, float Damage)
	 * @brie충돌에 대한 위치와 데미지 처리를 위한 가상함수
	 * @return bool
	 * 반환에 true 맞은것 false 막은것
	 */
	virtual bool HitDecision(eHitDecision hitPosition, float Damage) const = 0;

	/**
	 * @brie Actor 의 위치가 아닌 상대가 바라보고 있어야 하는 위치를 반환한다.
	 * @return 얼굴 혹은 특정 위치의 소켓 혹은 본 Location 을 반환하는 식을 완성한다.
	 */
	virtual FVector GetLocationForLook() const = 0;


};
