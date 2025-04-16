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

	// 키 씹힘 방지를 위해, 0.05초 딜레이 후 몽타주 재생
	GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &UGA_Player_Roll::PlayRollMontage, 0.05f, false);
}

void UGA_Player_Roll::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Player_Roll::PlayRollMontage()
{
	// 모션 워핑을 통해, 구르기 방향 및 거리 설정
	ComputeRollDirectionAndDistance();

	// 몽타주 플레이 태스크 초기화
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, RollMontage);
	if (MontageTask)
	{
		// 몽타주 혼합, 완료, 중단 취소에 EndAbility 바인딩
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Player_Roll::K2_EndAbility);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_Player_Roll::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Player_Roll::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Player_Roll::K2_EndAbility);

		// 몽타주 플레이 태스크 활성화
		MontageTask->ReadyForActivation();
	}
}

void UGA_Player_Roll::ComputeRollDirectionAndDistance()
{
	// 캐릭터에 마지막으로 입력받은 이동 입력의 정규화된 벡터 가져옴
	FVector Direction = GetPlayerCharacterFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();

	// 구르기 회전값 설정
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
