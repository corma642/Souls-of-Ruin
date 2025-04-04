#pragma once

#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "PA_StructTypes.generated.h"

// 플레이어 무기 어빌리티 세트
USTRUCT(BlueprintType)
struct FPlayerWeaponAbilitySet
{
	GENERATED_BODY()

public:
	// 어빌리티 입력 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "InputTag"))
	FGameplayTag InputTag;

	// 어빌리티
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UPA_PlayerGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};


// 플레이어 무기 데이터
USTRUCT(BlueprintType)
struct FPlayerWeaponData
{
	GENERATED_BODY()

public:
	// 무기 애님 레이어 링크
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TSubclassOf<class UPA_PlayerLinkedAnimLayer> WeaponAnimLayerToLink;

	// 무기 입력 맵핑 콘텍스트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TObjectPtr<class UInputMappingContext> WeaponInputMappingContext;

	// 기본 부여 무기 어빌리티 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TArray<FPlayerWeaponAbilitySet> DefaultWeaponAbilities;

	// 기본 무기 피해량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	float BaseWeaponDamage;
};
