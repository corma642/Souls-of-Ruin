// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PA_EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "PA_GameplayTags.h"

void UPA_EnemyCombatComponent::OnWeaponHitStartTargetActor(AActor* HitActor)
{
	// �̹� ������ �迭�� �����ϴ� ���ʹ� ����
	if (OverlappedActors.Contains(HitActor)) return;

	OverlappedActors.AddUnique(HitActor);

	// �����÷��� �̺�Ʈ �����͸� �����, �浹�� ����� ����
	FGameplayEventData Payload;
	Payload.Instigator = GetOwner<APawn>();
	Payload.Target = HitActor;

	// �ڽſ��� �����÷��� �̺�Ʈ ����
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner<APawn>(), PA_GameplayTags::Shared_Event_MeleeHit, Payload);
}

void UPA_EnemyCombatComponent::OnWeaponHitEndTargetActor(AActor* HitActor)
{
}
