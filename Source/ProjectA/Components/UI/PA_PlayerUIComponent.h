// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PA_PawnUIComponent.h"
#include "GameplayTagContainer.h"
#include "PA_PlayerUIComponent.generated.h"

// 무기 스킬 아이콘 변경 업데이트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnWeaponSkillIconUpdateDelegate,
	FGameplayTag, AbilityInputTag,	// 어빌리티 입력 태그
	TSoftObjectPtr<UMaterialInterface>, SoftWeaponSkillIconMaterial // 무기 스킬 아이콘 머티리얼
);

// 무기 스킬 쿨타임 시작 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
	FOnWeaponSkillCooldownBeginDelegate, 
	FGameplayTag, AbilityInputTag,	// 어빌리티 입력 태그
	float, TotalCooldownTime,		// 총 재사용 대기 시간
	float, RemainingCooldownTime	// 남은 재사용 대기 시간
);

UCLASS()
class PROJECTA_API UPA_PlayerUIComponent : public UPA_PawnUIComponent
{
	GENERATED_BODY()

public:
	// 현재 스태미나 변경 델리게이트 이벤트
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangeDelegate OnCurrentStaminaChanged;

	// 무기 스킬 아이콘 변경 델리게이트 이벤트
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnWeaponSkillIconUpdateDelegate OnWeaponSkillIconUpdated;

	// 무기 스킬 쿨타임 시작 델리게이트 이벤트
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnWeaponSkillCooldownBeginDelegate OnWeaponSkillCooldownBegin;
};
