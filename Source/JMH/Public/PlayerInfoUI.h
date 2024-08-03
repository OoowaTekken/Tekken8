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
	UPROPERTY()
	class AGameMode_MH* gm;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_NameA;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_NameB;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_EndHPA;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UTextBlock* text_EndHPB;

	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_PlayerA;
	
	UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_PlayerB;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint1A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint2A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint3A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint1B;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint2B;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UImage* img_VictoryPoint3B;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HPBarA;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HitBarA;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HPBarB;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UProgressBar* Pro_HitBarB;
	
	
	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* HP_A;

	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UCanvasPanel* HP_B;

	// pual=1,kazuya=2,jin=3
	UFUNCTION()
	void SetPlayerinfo(ACharacter* PlayerA,ACharacter* PlayerB,int32 playerAnum,int32 playerBnum);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText KAZUYA = FText::FromString("KAZUYA");

	UPROPERTY(EditAnywhere,Category="MySettings")
	UTexture* img_kazuya;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FText PAUL = FText::FromString("PAUL");
	
	UPROPERTY(EditAnywhere,Category="MySettings")
	UTexture* img_paul;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString JIN = "JIN";

	//UPROPERTY(EditAnywhere,meta=(BindWidget),Category="MySettings")
	//TSubclassOf<UTexture> jin;
	
	//UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Round")
	//int32 playerAnum;

	//UPROPERTY(EditDefaultsOnly , BlueprintReadWrite , Category = "Round")
	//int32 playerBnum;

	//UPROPERTY(EditAnywhere , BlueprintReadWrite)
	//class ACPP_Tekken8CharacterParent* playerA;

	//UPROPERTY(EditAnywhere , BlueprintReadWrite)
	//class ACPP_Tekken8CharacterParent* playerB;
	
	// 플레이어 HitBar를 업데이트하는 함수
	//UFUNCTION(BlueprintCallable, Category = "UI")
	//void UpdateHitBar(float CurrentHit, float MaxHit);

	// 플레이어 HP를 업데이트하는 함수
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealthBars(float Player1CurrentHP, float Player1MaxHP, float Player2CurrentHP, float Player2MaxHP);

	float CalculateHealthPercentage(float CurrentHP, float MaxHP) const;
	
	//메쉬를 선택한다 (start UI)에서
	//- 1P와 2P의 선택값
	//- 캐릭터 이름(이미지,메쉬,이름),
	// 셋팅 함수(캐릭이름) 
	//- 카즈야, 폴 -> 그냥 BP를 가지고와 메쉬,이미지,이름. 셋팅 되어있는 ㅇㅇ 캐릭터 c++ 상속받은 BP //근데 이러면 특수모드도 구현하기 쉬..울??

	
};
