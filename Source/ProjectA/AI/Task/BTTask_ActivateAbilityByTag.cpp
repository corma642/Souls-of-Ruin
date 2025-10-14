// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_ActivateAbilityByTag.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/PA_CharacterEnemy.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AIController.h"
#include "MotionWarpingComponent.h"
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

	bNotifyTick = true;				// 태스크 알림 허용
	bNotifyTaskFinished = true;		// 태스크 완료 알림 허용
	bCreateNodeInstance = false;	// 태스크의 인스턴스화 비활성화

	bUseMotionWarping = false;
	bIsCanStopAttack = true;
}

EBTNodeResult::Type UBTTask_ActivateAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// 공격 중 상태 키를 true로 설정
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsAttackingKey.SelectedKeyName, true);

	// AI 캐릭터 가져오기
	AICharacter = Cast<APA_CharacterEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (!AICharacter)
	{
		return EBTNodeResult::Failed;
	}

	// 공격 대상 가져오기
	TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	// 적 공격 어빌리티 활성화
	Cast<UPA_AbilitySystemComponent>(AICharacter->GetAbilitySystemComponent())->TryActivateAbilityByTag(AbilityTagToActivate);

	return EBTNodeResult::InProgress; // 태스크 진행중으로 종료
	// TickTask 함수에서 이후 처리
}

void UBTTask_ActivateAbilityByTag::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// AI 캐릭터나 공격 대상이 유효하지 않으면 태스크를 실패로 종료
	if (!AICharacter || !TargetActor)
	{
		FinishTask(OwnerComp, false);
	}

	// 공격 중 피해를 받았을 때 공격이 중단될 수 있는 경우
	if (bIsCanStopAttack)
	{
		// 공격받으면 태스크를 실패로 종료
		if (UPA_FunctionLibrary::NativeDoesActorHaveTag(AICharacter, PA_GameplayTags::Enemy_Status_UnderAttack))
		{
			FinishTask(OwnerComp, false);
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
				// #1: 최종 목표 위치 계산
				const FVector FinalTargetLocation = GetLocationWarpTarget();

				// #2: 최종 목표 위치 업데이트
				AICharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
					"LocationTarget",
					FinalTargetLocation
				);

				// #3: 공격 모션 워핑 회전 업데이트
				AICharacter->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocation(
					"RotationTarget",
					GetRotationWarpTarget()
				);
			}
		}
	}
}

FVector UBTTask_ActivateAbilityByTag::GetLocationWarpTarget()
{
	if (TargetActor == nullptr) return FVector();

	const FVector Location = AICharacter->GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation();

	// 목표 위치로의 방향을 구함
	FVector TargetToMe = (Location - TargetLocation).GetSafeNormal();

	// 목표 위치로의 방향에 떨어질 거리를 곱해서 목표 위치로부터의 떨어진 거리를 구함
	TargetToMe *= WarpTargetDistance;

	// 목표 위치에 내 방향으로부터 떨어질 거리를 더한 위치를 반환
	return TargetLocation + TargetToMe;
}

FVector UBTTask_ActivateAbilityByTag::GetRotationWarpTarget()
{
	return TargetActor->GetActorLocation();
}

void UBTTask_ActivateAbilityByTag::FinishTask(UBehaviorTreeComponent& OwnerComp, bool bIsSucceeded)
{
	// 공격 중 상태를 false로 설정
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(IsAttackingKey.SelectedKeyName, false);

	bIsSucceeded ? FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded) : FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}
