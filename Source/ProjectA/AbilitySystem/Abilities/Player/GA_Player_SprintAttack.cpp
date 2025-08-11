// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/GA_Player_SprintAttack.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "PA_FunctionLibrary.h"
#include "PA_GameplayTags.h"

UGA_Player_SprintAttack::UGA_Player_SprintAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Player_SprintAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// �޸��� ���� �±� ���� ����
	bool bHaveSprintingTag = UPA_FunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), PA_GameplayTags::Player_Status_Sprinting);

	// �̵� �ӵ��� 550�̻��� ���
	bool bSatisfySpeed = Cast<ACharacter>(ActorInfo->AvatarActor.Get())->GetVelocity().Size() >= 550.0f;

	if (!bHaveSprintingTag || !bSatisfySpeed)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
	}
	else
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		
		PlaySprintAttackMontage();
	}
}

void UGA_Player_SprintAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Player_SprintAttack::PlaySprintAttackMontage()
{
	// ��� ������ ����, ���� ���� �� �Ÿ� ����
	ComputeAttackDistance();

	// ��Ÿ�� �÷��� �½�ũ �ʱ�ȭ
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SprintAttackMontage);
	if (MontageTask)
	{
		// ��Ÿ�� ȥ��, �Ϸ�, �ߴ� ��ҿ� EndAbility ���ε�
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Player_SprintAttack::K2_EndAbility);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_Player_SprintAttack::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Player_SprintAttack::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Player_SprintAttack::K2_EndAbility);

		// ��Ÿ�� �÷��� �½�ũ Ȱ��ȭ
		MontageTask->ReadyForActivation();
	}
}

void UGA_Player_SprintAttack::ComputeAttackDistance()
{
	FHitResult HitResult;

	FVector TraceStart = GetPlayerCharacterFromActorInfo()->GetActorLocation() + (GetPlayerCharacterFromActorInfo()->GetActorForwardVector() * AttackDistance);
	FVector TraceEnd = TraceStart + (GetPlayerCharacterFromActorInfo()->GetActorUpVector() * -1 * 500.f);

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		TraceStart,
		TraceEnd,
		ObjectTypesQuery,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("AttackLocation"),
			HitResult.ImpactPoint
		);
	}
	else
	{
		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->RemoveWarpTarget(TEXT("RollingLocation"));
	}
}
