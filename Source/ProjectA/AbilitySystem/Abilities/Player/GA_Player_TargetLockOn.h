// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"
#include "GA_Player_TargetLockOn.generated.h"

UCLASS()
class PROJECTA_API UGA_Player_TargetLockOn : public UPA_PlayerGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	// 락온 시도 함수
	void TryLockOnTarget();

	// 고정 대상 확보 함수
	void GetAvailableActorsToLock();

	// 락온 취소 함수
	void CancelTargetLockOnAbility();

	// 데이터 정리 함수
	void ClaenUp();

	// 락온 가능한 대상 중, 특정 대상을 기준으로 가장 근접한 대상을 반환하는 함수
	AActor* GetNearestTargetFromAvailableActors(const AActor* TargetActor, const TArray<AActor*>& InVailableActors);

	// 타깃 락온 위젯 그리는 함수
	void DrawTargetLockOnWidget();

	// 타깃 락온 위젯 위치 설정 함수 (2D)
	void SetTargetLockOnWidgetPosition();

	// 락온 가능 대상 주변 좌/우 타깃 구하기
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);

	// 락온 입력 맵핑 콘텍스트 초기화
	void InitTargetLockOnMappingContext();

	// 락온 입력 맵핑 콘텍스트 제거
	void ResetTargetLockOnMappingContext();

protected:
	// 타깃 락온 틱 함수
	UFUNCTION(BlueprintCallable)
	void OnTargetLockOnTick(float DeltaTime);

	// 타깃 락온이 가능한지 확인하는 함수
	bool CanOnTargetLockOnTick();

	// 락온 대상을 향해 회전하는 함수
	void TargetLockOnRotate(float DeltaTime);

	// 락온 대상 변경 함수
	UFUNCTION(BlueprintCallable)
	void ChangeLockOnTarget(const FGameplayTag& InChangeDirectionTag);


	///////////////////////////////////////////////////////////////////////////
	/* 트레이싱 변수 */
private:
	// 박스 트레이스 충돌 길이
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability | TargetLockOn")
	float BoxTraceDistance = 2000.0f;

	// 박스 트레이스 크기
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability | TargetLockOn")
	FVector TraceBoxSize = FVector(500.0f, 750.0f, 1500.0f);

	// 충돌 감지 채널
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability | TargetLockOn")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTraceChannel;

	// 출력 표시 여부
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability | TargetLockOn")
	bool bShowPersistentDebugShape = false;


	///////////////////////////////////////////////////////////////////////////
	/* 락온 변수 */
private:
	// 타깃 고정 가능 액터 배열
	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	// 현재 락온 중인 액터
	UPROPERTY()
	AActor* CurrentLockedActor;

	// 락온 액터로의 회전 보간 속도
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability | TargetLockOn")
	float TargetLockOnRotationInterpSpeed = 5.0f;

	// 락온 유지 거리
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability | TargetLockOn")
	float TargetLockOnKeepingDistance = 1000.0f;

	// 락온 대상 변경 입력 맵핑 콘텍스트
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability | TargetLockOn")
	TObjectPtr<class UInputMappingContext> TargetLockOnMappingContext;


	///////////////////////////////////////////////////////////////////////////
	/* 락온 위젯 변수 */
private:
	// 타깃 락온 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability | TargetLockOn")
	TSubclassOf<class UPA_BaseWidget> TargetLockOnWidgetClass;

	// 타깃 락온 위젯
	UPROPERTY()
	TObjectPtr<class UPA_BaseWidget> DrawnTargetLockOnWidget;

	// 타깃 고정 위젯 크기
	UPROPERTY()
	FVector2D TargetLockOnWidgetSize = FVector2D::ZeroVector;
};
