// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PA_EntrySequenceBase.generated.h"

// 위젯 가시화 기록 구조체
USTRUCT()
struct FWidgetVisibilityRecord
{
	GENERATED_BODY()

	TWeakObjectPtr<UUserWidget> Widget;	// 위젯 클래스
	ESlateVisibility PrevVisibility;	// 이전 가시화 여부
};

UCLASS()
class PROJECTA_API APA_EntrySequenceBase : public AActor
{
	GENERATED_BODY()

public:
	APA_EntrySequenceBase();

protected:
	// 시퀀스 재생 콜리전 오버랩 함수
	UFUNCTION()
	void OnPlaySequenceCollisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// 시퀀스 재생 함수
	void PlayEntroSequence(ACharacter* PlayerCharacter);

	// 시퀀스 종료 함수
	UFUNCTION()
	void OnSequenceFinished();

	// 시퀀스 재생 중에 설정 세팅 함수 (입력, UI 등)
	void SetUpSequenceSettings(APlayerController* Controller);
	void UnSetUpSequenceSettings(APlayerController* Controller);

protected:
	// 시퀀스 재생 콜리전 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
	TObjectPtr<class UBoxComponent> PlaySequenceCollisionBox;

	// 재생 시퀀스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom")
	TObjectPtr<class ULevelSequence> PlaySequence;

private:
	// 시퀀스 액터 저장
	UPROPERTY()
	class ALevelSequenceActor* SequenceActor;

	// 시퀀스 플레이어 저장
	UPROPERTY()
	class ULevelSequencePlayer* SequencePlayer;

	// PlayerCharacter 별로 숨긴 위젯과 이전 상태를 저장
	static TMap<TWeakObjectPtr<APlayerController>, TArray<FWidgetVisibilityRecord>> CachedWidgetStates;
};
