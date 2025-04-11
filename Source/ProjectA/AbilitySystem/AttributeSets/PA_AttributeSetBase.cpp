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
		// 캐릭터 클래스는 IPA_PawnUIInterface를 상속받음
		// 따라서 TWeakInterfacePtr 생성자 캐스팅으로 캐릭터의 UI 인터페이스 가져와 캐시 저장
		CachedPawnInterface = TWeakInterfacePtr<IPA_PawnUIInterface>(Data.Target.GetAvatarActor());
	}
	check(CachedPawnInterface.IsValid());

	// UI 인터페이스를 통해, UI 컴포넌트 가져옴
	UPA_PawnUIComponent* UIComponent = CachedPawnInterface->GetUIComponent();
	check(UIComponent);

	// 현재 체력 변경
	if (Data.EvaluatedData.Attribute == GetCurrentHealthAttribute())
	{
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(), 0, GetMaxHealth()));

		// 현재 체력 변경 브로드캐스트
		UIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());
	}

	// 현재 스태미나 변경
	else if (Data.EvaluatedData.Attribute == GetCurrentStaminaAttribute())
	{
		SetCurrentStamina(FMath::Clamp(GetCurrentStamina(), 0, GetMaxStamina()));

		// 현재 스태미나 변경 브로드캐스트
		if (UPA_PlayerUIComponent* PlayerUIComponent = CachedPawnInterface->GetPlayerUIComponent())
		{
			PlayerUIComponent->OnCurrentStaminaChanged.Broadcast(GetCurrentStamina() / GetMaxStamina());
		}
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
		// 현재 체력에서 받은 피해량 만큼 차감
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth() - GetDamageTaken(), 0.0f, GetMaxHealth()));

		// 현재 체력 변경 브로드캐스트
		UIComponent->OnCurrentHealthChanged.Broadcast(GetCurrentHealth() / GetMaxHealth());

		GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Cyan, FString::Printf(TEXT("MaxHP: %f, CurrentHP: %f"), GetMaxHealth(), GetCurrentHealth()));

		// 현재 체력이 0보다 작은 경우
		if (GetCurrentHealth() <= KINDA_SMALL_NUMBER)
		{
			// 사망 상태 태그 추가
			UPA_FunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), PA_GameplayTags::Shared_Status_Dead);
		}
	}
}
