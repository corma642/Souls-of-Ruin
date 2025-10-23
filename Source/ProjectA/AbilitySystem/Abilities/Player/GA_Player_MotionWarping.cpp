// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/GA_Player_MotionWarping.h"
#include "Characters/PA_CharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "MotionWarpingComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UGA_Player_MotionWarping::UGA_Player_MotionWarping()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bUseMotionWarping = true;

	GroundObjectTypes.AddUnique(TEnumAsByte<EObjectTypeQuery>(ECollisionChannel::ECC_WorldStatic));
	GroundObjectTypes.AddUnique(TEnumAsByte<EObjectTypeQuery>(ECollisionChannel::ECC_WorldDynamic));
	GroundObjectTypes.AddUnique(TEnumAsByte<EObjectTypeQuery>(ECollisionChannel::ECC_Pawn));

	CollisionChannelToIgnore.AddUnique(ECollisionChannel::ECC_Pawn);
}

void UGA_Player_MotionWarping::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 콜리전 및 무브먼트 컴포넌트 상태 변경
	SetUpMotionWarpingState();

	// 몽타주 재생 함수 호출
	MontageToPlay();
}

void UGA_Player_MotionWarping::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// 태스크 종료
	if (IsValid(MontageTask))
	{
		MontageTask->EndTask();
	}

	// 콜리전 및 무브먼트 컴포넌트 상태 변경 복구
	UnSetUpMotionWarpingState();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
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

	// 플레이어 전방으로 이동 거리만큼 트레이싱 수행
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
		// ImpactPoint에서 캐릭터 방향의 역방향으로 Offset을 적용 (겹침 방지)
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

void UGA_Player_MotionWarping::SetUpMotionWarpingState()
{
	ACharacter* Character = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	if (!Character)
	{
		return;
	}

	// 캐릭터의 움직임 상태를 나는 상태로 변경
	// Root Motion의 Z축 움직임을 활성화하기 위함
	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
	if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Flying);
	}

	// 충돌을 무시할 콜리전 채널을 모두 Ignore로 설정
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	if (CapsuleComponent)
	{
		for (auto Channel : CollisionChannelToIgnore)
		{
			// 변경한 콜리전 채널 저장 (복구용)
			ChangedCollisionChannel.Add(Channel, CapsuleComponent->GetCollisionResponseToChannel(Channel));

			// 콜리전 채널 변경
			CapsuleComponent->SetCollisionResponseToChannel(Channel, ECollisionResponse::ECR_Ignore);
		}
	}
}

void UGA_Player_MotionWarping::UnSetUpMotionWarpingState()
{
	ACharacter* Character = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	if (!Character)
	{
		ChangedCollisionChannel.Reset();
		return;
	}

	// 캐릭터의 움직임 상태를 추락 상태로 변경
	UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
	if (CharacterMovement && CharacterMovement->IsFlying())
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Falling);
	}

	// 충돌을 무시했던 콜리전 채널을 모두 복구
	UCapsuleComponent* CapsuleComponent = Character->GetCapsuleComponent();
	if (CapsuleComponent)
	{
		for (auto Channel : ChangedCollisionChannel)
		{
			// 콜리전 채널 복구
			CapsuleComponent->SetCollisionResponseToChannel(Channel.Key, Channel.Value);
		}
	}

	ChangedCollisionChannel.Reset();
}
