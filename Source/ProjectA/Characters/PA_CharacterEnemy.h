// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PA_CharacterBase.h"
#include "PA_CharacterEnemy.generated.h"

UCLASS()
class PROJECTA_API APA_CharacterEnemy : public APA_CharacterBase
{
	GENERATED_BODY()

public:
	APA_CharacterEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;


	///////////////////////////////////////////////////////////////////////////
	/* StartUpData */
private:
	// 적 시작 데이터 초기화
	void InitEnemyStartUpData();


	///////////////////////////////////////////////////////////////////////////
	/* Combat */
public:
	FORCEINLINE class UPA_EnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }

	// 적 전투 컴포넌트 가져오기 인터페이스 함수
	/* ICombatInterface Interface */
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

	// 적 사망 함수 [사망 머티리얼 효과 재생 시간, 효과 재생 간격, 사망 디졸브 나이아가라 시스템, 나이아가라 디졸브 색상]
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	void OnEnemyDied(float InDuration, float InUpdateInterval, TSoftObjectPtr<class UNiagaraSystem> DissolveNiagara, FLinearColor DissolveNiagaraColor);

private:
	// 사망 머티리얼 효과 (디졸브) 함수
	void UpdateMaterialParameter(float InDuration, float InUpdateInterval);
	void UpdateMaterialParameterCallback();

	// 사망 머티리얼 효과 (디졸브) 경과 시간
	float ElapsedTime = 0.0f;

	// 사망 머티리얼 효과 (디졸브) 총 재생 시간
	float Duration = 0.0f;

	// 사망 머티리얼 효과 (디졸브) 업데이트 간격
	float UpdateInterval = 0.0f;

	// 사망 머티리얼 효과 (디졸브) 타이머
	FTimerHandle EnemyDiedTimerHandle;

	UPROPERTY()
	class APA_BaseWeapon* EnemyWeapon;

	// 적 전투 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPA_EnemyCombatComponent> EnemyCombatComponent;


	///////////////////////////////////////////////////////////////////////////
	/* UI & Widget */
public:
	// 적 UI 컴포넌트 가져오기 인터페이스 함수
	/* IPA_PawnUIInterface Interface */
	virtual UPA_PawnUIComponent* GetUIComponent() const override;
	virtual UPA_EnemyUIComponent* GetEnemyUIComponent() const override;
	/* IPA_PawnUIInterface Interface */

protected:
	// 적 UI 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | UI")
	TObjectPtr<class UPA_EnemyUIComponent> EnemyUIComponent;

	// 적 체력 바 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | UI")
	TObjectPtr<class UWidgetComponent> EnemyHealthWidgetComponent;
};
