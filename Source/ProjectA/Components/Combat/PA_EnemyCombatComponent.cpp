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
	bool bIsValidBlock = IsValidBlock(HitActor);

	// 이벤트 전달 객체
	FGameplayEventData Payload;
	Payload.Instigator = GetOwner<APawn>();
	Payload.Target = HitActor;

	// 피격 HitResult 보내기
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

bool UPA_EnemyCombatComponent::IsValidBlock(AActor* HitActor)
{
	bool bIsValidBlock = false;

	// 플레이어에게 "막는 중" 태그 보유중인지 검사
	const bool bIsPlayerBlocking = UPA_FunctionLibrary::NativeDoesActorHaveTag(HitActor, PA_GameplayTags::Player_Status_Blocking);

	// 내 공격이 막기를 뚫는 공격인지 검사
	const bool bIsMyAttackUnblockable = false;

	// 플레이어는 막는 중인데, 내 공격은 막기를 뚫지 못할 경우
	if (bIsPlayerBlocking && !bIsMyAttackUnblockable)
	{
		// 공격자와 피해자의 두 전방 벡터의 내적을 구함
		const float DotResult = FVector::DotProduct(HitActor->GetActorForwardVector(), GetOwner()->GetActorForwardVector());

		// -1이 완전히 마주 보는 상태, 0이 수직인 상태이니, -0.4정도면 막기 성공
		bIsValidBlock = DotResult < -0.4f;
	}

	return bIsValidBlock;
}
