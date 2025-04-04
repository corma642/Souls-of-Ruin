#pragma once

#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "PA_StructTypes.generated.h"

// �÷��̾� ���� �����Ƽ ��Ʈ
USTRUCT(BlueprintType)
struct FPlayerWeaponAbilitySet
{
	GENERATED_BODY()

public:
	// �����Ƽ �Է� �±�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Category = "InputTag"))
	FGameplayTag InputTag;

	// �����Ƽ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UPA_PlayerGameplayAbility> AbilityToGrant;

	bool IsValid() const;
};


// �÷��̾� ���� ������
USTRUCT(BlueprintType)
struct FPlayerWeaponData
{
	GENERATED_BODY()

public:
	// ���� �ִ� ���̾� ��ũ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TSubclassOf<class UPA_PlayerLinkedAnimLayer> WeaponAnimLayerToLink;

	// ���� �Է� ���� ���ؽ�Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TObjectPtr<class UInputMappingContext> WeaponInputMappingContext;

	// �⺻ �ο� ���� �����Ƽ �迭
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TArray<FPlayerWeaponAbilitySet> DefaultWeaponAbilities;

	// �⺻ ���� ���ط�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	float BaseWeaponDamage;
};
