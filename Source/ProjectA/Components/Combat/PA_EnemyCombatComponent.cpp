// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "PA_GameplayTags.h"

void UPA_EnemyCombatComponent::OnWeaponHitStartTargetActor(AActor* HitActor)
{
	// 이미 오버랩 배열에 존재하는 액터는 무시
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	// 게임플레이 이벤트 데이터를 만들어, 충돌한 대상을 저장
	FGameplayEventData Payload;
	Payload.Instigator = GetOwner<APawn>();
	Payload.Target = HitActor;

	// 자신에게 게임플레이 이벤트 전달
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Shared_Event_MeleeHit, Payload);
}

void UPA_EnemyCombatComponent::OnWeaponHitEndTargetActor(AActor* HitActor)
{
}
