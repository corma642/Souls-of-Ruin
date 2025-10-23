// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GameplayTagContainer.h"
#include "AT_WaitSpawnEnemies.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate, const TArray<class APA_CharacterEnemy*>&, SpawnedEnemies);

UCLASS()
class PROJECTA_API UAT_WaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAT_WaitSpawnEnemies();

	// 팩토리 함수. AbilityTask를 생성/실행할 때 사용되는 정적 함수
	UFUNCTION(BlueprintCallable, Category = "Custom | AbilityTasks", meta = (HidePin = "OwningAbility",
		DefaultToSelf = "OwningAbility",
		BlueprintInternalUseOnly = "true",
		NumToSpawn = "1",
		MinSpawnRadius = "200",
		MaxSpawnRadius = "500")
	)
	/*
	* OwningAbility			: 소유자 어빌리티
	* EventTag				: 이벤트 태그
	* SoftEnemyClassToSpawn	: 소환할 적 클래스(약 참조) 배열
	* NumToSpawn			: 소환할 적의 수
	* MinSpawnExtent		: 최소 소환 범위
	* MaxSpawnExtent		: 최대 소환 범위
	*/
	static UAT_WaitSpawnEnemies* WaitSpawnEnemies(
		UGameplayAbility* OwningAbility,
		FGameplayTag EventTag,
		TArray<TSoftClassPtr<class APA_CharacterEnemy>> SoftEnemyClassToSpawn,
		const int32 NumToSpawn,
		const float MinSpawnExtent,
		const float MaxSpawnExtent
	);

	// 태스크 트리거 시 호출되는 함수
	virtual void Activate() override;

	// 태스크 종료/제거 시 호출되는 함수
	virtual void OnDestroy(bool bInOwnerFinished) override;

	// 이벤트 태그 트리거 시 호출되는 함수
	void OnGameplayEventReceived(const FGameplayEventData* InPayload);

	// 소환 위치를 구하는 함수
	FVector GetSpawnLocationInRandRange();

	// 적 소환 실패 함수
	void SpawnFailed();

private:
	// 적 비동기 로딩 후, 적 소환 콜백 함수
	void OnEnemyClassLoaded();

public:
	// 소환 완료 델리게이트 변수
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnSpawnFinished;

	// 소환 실패 델리게이트 변수
	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnDidNotSpawn;

	// 생성한 델리게이트를 관리하는 핸들 변수
	FDelegateHandle DelegateHandle;

private:
	// 변수 캐시 저장
	FGameplayTag CachedEventTag;
	TArray<TSoftClassPtr<class APA_CharacterEnemy>> CachedSoftEnemyClassToSpawn;
	int32 CachedNumToSpawn;
	float CachedMinSpawnExtent;
	float CachedMaxSpawnExtent;
};
