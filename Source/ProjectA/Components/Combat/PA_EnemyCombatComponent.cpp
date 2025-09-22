// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_EnemyCombatComponent.h"
#include "Characters/PA_CharacterEnemy.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PA_FunctionLibrary.h"
#include "AbilitySystemComponent.h"

#include "PA_GameplayTags.h"

UPA_EnemyCombatComponent::UPA_EnemyCombatComponent()
{
	Enemy = Cast<APA_CharacterEnemy>(GetOwner());
}

void UPA_EnemyCombatComponent::OnWeaponHitStartTargetActor(AActor* HitActor, const FHitResult& HitResult)
{
	// 이미 오버랩 배열에 존재하는 액터는 무시
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	UAbilitySystemComponent* ASC = Enemy->GetAbilitySystemComponent();

	// 막기 유효 여부
	bool bIsValidBlock = UPA_FunctionLibrary::NativeIsValidBlock(GetOwner(), HitActor);

	// 이벤트 전달 객체
	FGameplayEventData Payload;
	Payload.Instigator = GetOwner<APawn>();
	Payload.Target = HitActor;

	// 피격 HitResult
	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddHitResult(HitResult, false);
	Payload.ContextHandle = EffectContext;

	if (bIsValidBlock)
	{
		GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Green, FString("Successed Block!!"));

		// 막기 성공. 피격 액터에게 막기 성공 이벤트 전달
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor, PA_GameplayTags::Player_Event_SuccessfulBlock, Payload);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(5, 3.f, FColor::Red, FString("Failed Block!!"));

		// 막기 실패. 피해 이벤트 전달
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Shared_Event_MeleeHit, Payload);
	}
}

void UPA_EnemyCombatComponent::OnWeaponHitEndTargetActor(AActor* HitActor, const FHitResult& HitResult)
{
}
