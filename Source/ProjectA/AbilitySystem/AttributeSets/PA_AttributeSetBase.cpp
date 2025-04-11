// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"
#include "GameplayEffectExtension.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/UI/PA_PawnUIComponent.h"
#include "Components/UI/PA_PlayerUIComponent.h"
#include "Components/UI/PA_EnemyUIComponent.h"
#include "Interface/PA_PawnUIInterface.h"

#include "PA_FunctionLibrary.h"
#include "PA_GameplayTags.h"

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

	if (!CachedPawnInterface.IsValid())
	{
		// ĳ���� Ŭ������ IPA_PawnUIInterface�� ��ӹ���
		// ���� TWeakInterfacePtr ������ ĳ�������� ĳ������ UI �������̽� ������ ĳ�� ����
		CachedPawnInterface = TWeakInterfacePtr<IPA_PawnUIInterface>(Data.Target.GetAvatarActor());
	}
	check(CachedPawnInterface.IsValid());

	// UI �������̽��� ����, UI ������Ʈ ������
	UPA_PawnUIComponent* UIComponent = CachedPawnInterface->GetUIComponent();
	check(UIComponent);

	// ���� ü�� ����
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0, GetMaxHealth()));

		// ���� ü�� ���� ��ε�ĳ��Ʈ
		UIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	// ���� ���¹̳� ����
	else if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		SetCurrentStamina(FMath::Clamp(GetCurrentStamina(), 0, GetMaxStamina()));

		// ���� ���¹̳� ���� ��ε�ĳ��Ʈ
		if (UPA_PlayerUIComponent* PlayerUIComponent = CachedPawnInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentStaminaChanged.Broadcast(GetCurrentStamina() / GetMaxStamina());
		}
	}

	// �ִ� �̵��ӵ� ����
	else if (Data.EvaluatedData.Attribute == GetMaxMovementSpeedAttribute())
	{
		ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActor());
		UCharacterMovementComponent* CharacterMovement = OwningCharacter ? OwningCharacter->GetCharacterMovement() : nullptr;

		if (CharacterMovement)
		{
			CharacterMovement->MaxWalkSpeed = GetMaxMovementSpeed();
		}
	}

	// ���� ���ط� ����
	else if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		// ���� ü�¿��� ���� ���ط� ��ŭ ����
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetDamageTaken(), 0.0f, GetMaxHealth()));

		// ���� ü�� ���� ��ε�ĳ��Ʈ
		UIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Cyan, FString::Printf(TEXT("MaxHP: %f, CurrentHP: %f"), GetMaxHealth(), GetCurrentHealth()));

		// ���� ü���� 0���� ���� ���
		if (GetCurrentHealth() <= KINDA_SMALL_NUMBER)
		{
			// ��� ���� �±� �߰�
			UPA_FunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), PA_GameplayTags::Shared_Status_Dead);
		}
	}
}
