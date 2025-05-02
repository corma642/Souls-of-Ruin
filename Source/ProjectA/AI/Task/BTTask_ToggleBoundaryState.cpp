// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_ToggleBoundaryState.h"
#include "Characters/PA_CharacterEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"

#include "PA_FunctionLibrary.h"
#include "PA_GameplayTags.h"

UBTTask_ToggleBoundaryState::UBTTask_ToggleBoundaryState()
{
	NodeName = "Toggle Boundary State";

	INIT_TASK_NODE_NOTIFY_FLAGS();

	bShouldEnable = false;
	bShouldChangeMaxWalkSpeed = false;
	BoundaryWalkSpeed = 0.0f;
}

EBTNodeResult::Type UBTTask_ToggleBoundaryState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_ToggleBoundaryState::OnExecuteTask()
{
	check(CachedEnemyCharacter.IsValid());

	// 적 캐릭터 무브먼트 컴포넌트가 유효하지 않은 경우
	if (!CachedMovementComp.IsValid())
	{
		// 캐스팅을 통한 첫 캐시 데이터 저장
		CachedMovementComp = CachedEnemyCharacter->GetCharacterMovement();
	}
	check(CachedMovementComp.IsValid());

	// 경계 상태 활성화
	if (bShouldEnable)
	{
		// 움직임에 따른 회전 비활성화 (락온)
		CachedMovementComp->bOrientRotationToMovement = false;

		// 경계 상태로 인한 최대 이동속도 변경
		if (bShouldChangeMaxWalkSpeed)
		{
			CachedMovementComp->MaxWalkSpeed = BoundaryWalkSpeed;
		}

		// 경계 상태 태그 추가
		UPA_FunctionLibrary::AddGameplayTagToActorIfNone(CachedEnemyCharacter.Get(), PA_GameplayTags::Enemy_Status_Boundary);
	}
	// 경계 상태 비활성화
	else
	{
		// 움직임에 따른 회전을 다시 활성화 (락온 해제)
		CachedMovementComp->bOrientRotationToMovement = true;

		// 최대 이동속도 원상복구
		if (!bShouldChangeMaxWalkSpeed)
		{
			if (AAIController* AIController = Cast<AAIController>(CachedEnemyCharacter->GetController()))
			{
				if (UBlackboardComponent* BBComp = AIController->GetBlackboardComponent())
				{
					CachedMovementComp->MaxWalkSpeed = CachedEnemyCharacter->GetAttributeSet()->GetMaxMovementSpeed();
				}
			}
		}

		// 경계 상태 태그 제거
		UPA_FunctionLibrary::RemoveGameplayTagToActorIfFound(CachedEnemyCharacter.Get(), PA_GameplayTags::Enemy_Status_Boundary);
	}
}
