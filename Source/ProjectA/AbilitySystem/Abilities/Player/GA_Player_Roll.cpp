// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/GA_Player_Roll.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MotionWarpingComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UGA_Player_Roll::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Ű ���� ������ ����, 0.05�� ������ �� ��Ÿ�� ���
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UGA_Player_Roll::PlayRollMontage, 0.05f, false);
}

void UGA_Player_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Player_Roll::PlayRollMontage()
{
	// ��� ������ ����, ������ ���� �� �Ÿ� ����
	ComputeRollDirectionAndDistance();

	// ��Ÿ�� �÷��� �½�ũ �ʱ�ȭ
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, RollMontage);
	if (MontageTask)
	{
		// ��Ÿ�� ȥ��, �Ϸ�, �ߴ� ��ҿ� EndAbility ���ε�
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Player_Roll::K2_EndAbility);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_Player_Roll::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Player_Roll::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Player_Roll::K2_EndAbility);

		// ��Ÿ�� �÷��� �½�ũ Ȱ��ȭ
		MontageTask->ReadyForActivation();
	}
}

void UGA_Player_Roll::ComputeRollDirectionAndDistance()
{
	// ĳ���Ϳ� ���������� �Է¹��� �̵� �Է��� ����ȭ�� ���� ������
	FVector Direction = GetPlayerCharacterFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();

	// ������ ȸ���� ����
	GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
		TEXT("RollingDirection"),
		FVector::ZeroVector,
		UKismetMathLibrary::MakeRotFromX(Direction)
	);


	FHitResult HitResult;

	FVector TraceStart = GetPlayerCharacterFromActorInfo()->GetActorLocation() + (Direction * RollDistance);
	FVector TraceEnd = TraceStart + (GetPlayerCharacterFromActorInfo()->GetActorUpVector() * -1 * 500.f);

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		TraceStart,
		TraceEnd,
		RollObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
			TEXT("RollingLocation"),
			HitResult.ImpactPoint
		);
	}
	else
	{
		GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->RemoveWarpTarget(TEXT("RollingLocation"));
	}
}
