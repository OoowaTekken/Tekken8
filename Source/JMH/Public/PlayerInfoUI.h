// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfoUI.generated.h"

/**
 * 
 */
UCLASS()
class JMH_API UPlayerInfoUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_NameA;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_EndHPA;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_PlayerA;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint1A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint2A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint3A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HPBarA;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HitBarA;


	UFUNCTION()
	void SetPlayerinfo(int CharacterNum);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString KAZUYA = "KAZUYA";

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString PAUL = "PAUL";

	

	
	

	
	
	//메쉬를 선택한다 (start UI)에서
	//- 1P와 2P의 선택값
	//- 캐릭터 이름(이미지,메쉬,이름),
	// 셋팅 함수(캐릭이름) 
	//- 카즈야, 폴 -> 그냥 BP를 가지고와 메쉬,이미지,이름. 셋팅 되어있는 ㅇㅇ 캐릭터 c++ 상속받은 BP //근데 이러면 특수모드도 구현하기 쉬..울??

	
};
