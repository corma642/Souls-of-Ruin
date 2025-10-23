// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_ActivateAbilityByTag.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/PA_CharacterEnemy.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AIController.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "PA_FunctionLibrary.h"
#include "PA_GameplayTags.h"

UBTTask_ActivateAbilityByTag::UBTTask_ActivateAbilityByTag()
{
	// BT에서 사용할 노드 이름 설정	
	NodeName = TEXT("BTTask Activate Ability By Tag");

	// / 필수 추가 / 기본 태스크가 제대로 동작하도록 플래그를 알리는 매크로
	INIT_TASK_NODE_NOTIFY_FLAGS();

	// 블랙보드 키 필터 설정
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorKey), AActor::StaticClass());

	GroundObjectTypes.AddUnique(TEnumAsByte<EObjectTypeQuery>(ECollisionChannel::ECC_WorldStatic));
	GroundObjectTypes.AddUnique(TEnumAsByte<EObjectTypeQuery>(ECollisionChannel::ECC_WorldDynamic));

	CollisionChannelToIgnore.AddUnique(ECollisionChannel::ECC_Pawn);

	bNotifyTick = true;				// 태스크 알림 허용
	bNotifyTaskFinished = true;		// 태스크 완료 알림 허용
	bCreateNodeInstance = false;	// 태스크의 인스턴스화 비활성화

	bUseMotionWarping = false;
	bUseZAxisRootMotion = false;
	bIsCanStopAttack = true;
}

EBTNodeResult::Type UBTTask_ActivateAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// AI 캐릭터 및 공격 대상 가져오기
	AICharacter = Cast<APA_CharacterEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!AICharacter || !TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	// 공격 중 상태 키를 true로 설정
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsAttackingKey.SelectedKeyName, true);

	// Z축 루트 모션 이동이 있는 경우
	if (bUseZAxisRootMotion)
	{
		// 콜리전 및 무브먼트 컴포넌트 상태 변경
		SetUpMotionWarpingState();
	}

	// 적 공격 어빌리티 활성화
	Cast<UPA_AbilitySystemComponent>(AICharacter->GetAbilitySystemComponent())->TryActivateAbilityByTag(AbilityTagToActivate);

	return EBTNodeResult::InProgress; // 태스크 진행중으로 종료
}

void UBTTask_ActivateAbilityByTag::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// AI 캐릭터나 공격 대상이 유효하지 않으면 태스크를 실패로 종료
	if (!AICharacter || !TargetActor)
	{
		FinishTask(OwnerComp, false);
		return;
	}

	// 공격 중 피해를 받았을 때 공격이 중단될 수 있는 경우 태스크를 실패로 종료
	if (bIsCanStopAttack)
	{
		if (UPA_FunctionLibrary::NativeDoesActorHaveTag(AICharacter, PA_GameplayTags::Enemy_Status_UnderAttack))
		{
			FinishTask(OwnerComp, false);
			return;
		}
	}

	// 공격 중 상태를 확인
	bool bIsAttacking = OwnerComp.GetBlackboardComponent()->GetValueAsBool(IsAttackingKey.SelectedKeyName);

	// 공격 중이 아니면 태스크를 완료
	if (!bIsAttacking)
	{
		if (bUseMotionWarping)
		{
			// 공격 모션 워핑 제거
			UMotionWarpingComponent* MotionWarpingComponent = AICharacter->GetMotionWarpingComponent();
			if (MotionWarpingComponent)
			{
				MotionWarpingComponent->RemoveWarpTarget(TEXT("LocationTarget"));
				MotionWarpingComponent->RemoveWarpTarget(TEXT("RotationTarget"));
			}
		}

		FinishTask(OwnerComp, true);
	}
	else
	{
		if (bUseMotionWarping)
		{
			// 공격 모션 워핑 업데이트 로직 수행
			UMotionWarpingComponent* MotionWarpingComponent = AICharacter->GetMotionWarpingComponent();
			if (MotionWarpingComponent)
			{
				// #1: 목표 위치 계산
				FVector FinalTargetLocation = FVector::ZeroVector;
				if (FVector::Dist(AICharacter->GetActorLocation(), TargetActor->GetActorLocation()) <= WarpTargetDistance)
				{
					FinalTargetLocation = AICharacter->GetActorLocation();
				}
				else
				{
					// 타깃 위치로부터 일정 거리 떨어지도록 보정(겹침 방지)
					FinalTargetLocation = TargetActor->GetActorLocation() - (AICharacter->GetActorForwardVector() * WarpTargetDistance);
				}

				// #2: 목표 위치 업데이트
				AICharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
					"LocationTarget",
					FinalTargetLocation
				);

				// #3: 공격 모션 워핑 회전 업데이트
				AICharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation(
					TEXT("RotationTarget"),
					TargetActor->GetActorLocation(),
					UKismetMathLibrary::MakeRotFromX((TargetActor->GetActorLocation() - AICharacter->GetActorLocation()).GetSafeNormal())
				);
			}
		}
	}
}

void UBTTask_ActivateAbilityByTag::FinishTask(UBehaviorTreeComponent& OwnerComp, bool bIsSucceeded)
{
	// Z축 루트 모션 이동이 있는 경우
	if (bUseZAxisRootMotion)
	{
		// 콜리전 및 무브먼트 컴포넌트 상태 변경 복구
		UnSetUpMotionWarpingState();
	}

	// 공격 중 상태를 false로 설정
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsAttackingKey.SelectedKeyName, false);

	bIsSucceeded ? FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded) : FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}

void UBTTask_ActivateAbilityByTag::SetUpMotionWarpingState()
{
	if (!AICharacter)
	{
		return;
	}

	// 캐릭터의 움직임 상태를 나는 상태로 변경
	// Root Motion의 Z축 움직임을 활성화하기 위함
	UCharacterMovementComponent* CharacterMovement = AICharacter->GetCharacterMovement();
	if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Flying);
	}

	// 충돌을 무시할 콜리전 채널을 모두 Ignore로 설정
	UCapsuleComponent* CapsuleComponent = AICharacter->GetCapsuleComponent();
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

void UBTTask_ActivateAbilityByTag::UnSetUpMotionWarpingState()
{
	if (!AICharacter)
	{
		ChangedCollisionChannel.Reset();
		return;
	}

	// 캐릭터의 움직임 상태를 추락 상태로 변경
	UCharacterMovementComponent* CharacterMovement = AICharacter->GetCharacterMovement();
	if (CharacterMovement && CharacterMovement->IsFlying())
	{
		CharacterMovement->SetMovementMode(EMovementMode::MOVE_Falling);
	}

	// 충돌을 무시했던 콜리전 채널을 모두 복구
	UCapsuleComponent* CapsuleComponent = AICharacter->GetCapsuleComponent();
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
