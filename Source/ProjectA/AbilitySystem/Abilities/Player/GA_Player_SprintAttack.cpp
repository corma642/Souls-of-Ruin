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
	// 달리기 상태 태그 보유 여부
	bool bHaveSprintingTag = UPA_FunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), PA_GameplayTags::Player_Status_Sprinting);

	// 이동 속도가 550이상인 경우
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
	// 모션 워핑을 통해, 공격 방향 및 거리 설정
	ComputeAttackDistance();

	// 몽타주 플레이 태스크 초기화
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SprintAttackMontage);
	if (MontageTask)
	{
		// 몽타주 혼합, 완료, 중단 취소에 EndAbility 바인딩
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Player_SprintAttack::K2_EndAbility);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_Player_SprintAttack::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Player_SprintAttack::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Player_SprintAttack::K2_EndAbility);

		// 몽타주 플레이 태스크 활성화
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
