// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/GA_Player_MotionWarping.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MotionWarpingComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UGA_Player_MotionWarping::UGA_Player_MotionWarping()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bUseMotionWarping = true;
}

void UGA_Player_MotionWarping::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 몽타주 재생 함수 호출
	MontageToPlay();
}

void UGA_Player_MotionWarping::MontageToPlay()
{
	if (bUseMotionWarping)
	{
		// 모션 워핑 방향 및 거리 설정
		ComputeDirectionAndDistance();
	}

	// 몽타주 플레이 태스크 초기화
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, PlayMontage);
	if (MontageTask)
	{
		// 몽타주 혼합, 완료, 중단 취소에 EndAbility 바인딩
		MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::K2_EndAbility);
		MontageTask->OnCompleted.AddDynamic(this, &ThisClass::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &ThisClass::K2_EndAbility);

		// 몽타주 플레이 태스크 활성화
		MontageTask->ReadyForActivation();
	}
}

void UGA_Player_MotionWarping::ComputeDirectionAndDistance()
{
	// 캐릭터에 마지막으로 입력받은 이동 입력의 정규화된 벡터 가져오기
	FVector Direction = GetPlayerCharacterFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();

	// 모션 워핑 회전값 설정
	GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
		TEXT("RotationTarget"),
		FVector::ZeroVector,
		UKismetMathLibrary::MakeRotFromX(Direction)
	);

	// 플레이어 전방으로 모션 워핑 이동 거리만큼 트레이싱 수행
	FHitResult HitResult;

	FVector TraceStart = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	FVector TraceEnd = TraceStart + (Direction * MoveDistance);

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		TraceStart,
		TraceEnd,
		GroundObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	// 최종 이동 위치
	FVector FinalTargetLocation = FVector::ZeroVector;

	// 이동 거리에 충돌체가 있다면 해당 위치로 이동 위치 업데이트
	if (HitResult.bBlockingHit)
	{
		// 충돌 지점에서 살짝 물러나도록 오프셋 조정
		const float DistanceOffset = 10.f;

		// ImpactPoint에서 캐릭터 방향의 역방향으로 Offset을 적용
		FinalTargetLocation = HitResult.ImpactPoint - (Direction * DistanceOffset);
	}
	// 이동 거리에 충돌체가 없는 경우
	else
	{
		// 이동 거리 끝 지점에 바닥이 존재하는지 확인
		// 바닥으로 트레이싱 수행
		FHitResult GroundHitResult;
		FVector GroundTraceStart = TraceEnd;
		FVector GroundTraceEnd = GroundTraceStart + (GetPlayerCharacterFromActorInfo()->GetActorUpVector() * -1 * 500.f);

		UKismetSystemLibrary::LineTraceSingleForObjects(
			this,
			GroundTraceStart,
			GroundTraceEnd,
			GroundObjectTypes,
			false,
			TArray<AActor*>(),
			EDrawDebugTrace::None,
			GroundHitResult,
			true
		);

		// 이동 거리 끝에 바닥이 존재하면 바닥으로 최동 이동 위치 설정
		if (GroundHitResult.bBlockingHit)
		{
			FinalTargetLocation = GroundHitResult.ImpactPoint;
		}
		else
		{
			// 바닥도 없으면 모션 워핑 취소
			GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->RemoveWarpTarget(TEXT("LocationTarget"));
			return;
		}
	}

	// 위치 업데이트
	GetPlayerCharacterFromActorInfo()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
		TEXT("LocationTarget"),
		FinalTargetLocation
	);
}
