// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"
#include "GameplayEffectExtension.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPA_AttributeSetBase::UPA_AttributeSetBase()
{
	InitCurrentHealth(1.0f);
	InitMaxHealth(1.0f);
	InitCurrentStamina(1.0f);
	InitMaxStamina(1.0f);
	InitMaxMovementSpeed(1.0f);
	InitAttackPower(1.0f);
	InitDefense(1.0f);
}

void UPA_AttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 현재 체력 변경
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0, GetMaxHealth()));
	}
	// 현재 스태미나 변경
	else if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		SetCurrentStamina(FMath::Clamp(GetCurrentStamina(), 0, GetMaxStamina()));
	}
	// 최대 이동속도 변경
	else if (Data.EvaluatedData.Attribute == GetMaxMovementSpeedAttribute())
	{
		ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor());
		UCharacterMovementComponent* CharacterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;

		if (CharacterMovement)
		{
			CharacterMovement->MaxWalkSpeed = GetMaxMovementSpeed();
		}
	}
	// 받은 피해량 변경
	else if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetDamageTaken(), 0.0f, GetMaxHealth()));

		GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Cyan, FString::Printf(TEXT("MaxHP: %f, CurrentHP: %f"), GetMaxHealth(), GetCurrentHealth()));
	}
}
