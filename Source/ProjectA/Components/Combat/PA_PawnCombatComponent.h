// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "PA_PawnCombatComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTA_API UPA_PawnCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPA_PawnCombatComponent();

	// ������ ���⸦ ����ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, class APA_BaseWeapon* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	// �±׸� ���� ĳ���Ϳ� ��ϵ� ���⸦ �������� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	class APA_BaseWeapon* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	// ���� ĳ���Ϳ� ������ ���⸦ �������� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	class APA_BaseWeapon* GetCharacterCurrentEquippingWeapon() const;

	// ���� ������ ������ �±�
	UPROPERTY(BlueprintReadWrite, Category = "Custom | Combat")
	FGameplayTag CurrentEquippingWeaponTag;

protected:
	// ���� ĳ���Ϳ� ��ϵ� ���� ��
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<class APA_BaseWeapon>> CharacterCarriedWeaponMap;
};
