// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "PA_PawnCombatComponent.generated.h"

// 무기 대미지 유형 토글
UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	None,
	LeftWeapon,
	RightWeapon,
	LeftHand,
	RightHand,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTA_API UPA_PawnCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPA_PawnCombatComponent();

	// 스폰된 무기를 등록하는 함수
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, class APA_BaseWeapon* InWeaponToRegister, bool bRegisterAsEquippedWeapon = false);

	// 태그를 통해 캐릭터에 등록된 무기를 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	class APA_BaseWeapon* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	// 현재 캐릭터에 장착된 무기를 가져오는 함수
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	class APA_BaseWeapon* GetCharacterCurrentEquippingWeapon() const;

public:
	// 무기 콜리전 전환 함수
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	void ToggleWeaponCollision(bool bShouldEnable, EToggleDamageType ToggleDamageType = EToggleDamageType::None);

	// 무기 피해 함수
	virtual void OnWeaponHitStartTargetActor(AActor* HitActor);
	
	// 무기 피해 종료 함수
	virtual void OnWeaponHitEndTargetActor(AActor* HitActor);
	
protected:
	// 현재 장착된 무기의 태그
	UPROPERTY(BlueprintReadWrite, Category = "Custom | Combat")
	FGameplayTag CurrentEquippingWeaponTag;

	// 현재 캐릭터에 등록된 무기 맵
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<class APA_BaseWeapon>> CharacterCarriedWeaponMap;
};
