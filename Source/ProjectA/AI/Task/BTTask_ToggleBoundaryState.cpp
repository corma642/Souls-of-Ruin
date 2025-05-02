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

	// �� ĳ���� �����Ʈ ������Ʈ�� ��ȿ���� ���� ���
	if (!CachedMovementComp.IsValid())
	{
		// ĳ������ ���� ù ĳ�� ������ ����
		CachedMovementComp = CachedEnemyCharacter->GetCharacterMovement();
	}
	check(CachedMovementComp.IsValid());

	// ��� ���� Ȱ��ȭ
	if (bShouldEnable)
	{
		// �����ӿ� ���� ȸ�� ��Ȱ��ȭ (����)
		CachedMovementComp->bOrientRotationToMovement = false;

		// ��� ���·� ���� �ִ� �̵��ӵ� ����
		if (bShouldChangeMaxWalkSpeed)
		{
			CachedMovementComp->MaxWalkSpeed = BoundaryWalkSpeed;
		}

		// ��� ���� �±� �߰�
		UPA_FunctionLibrary::AddGameplayTagToActorIfNone(CachedEnemyCharacter.Get(), PA_GameplayTags::Enemy_Status_Boundary);
	}
	// ��� ���� ��Ȱ��ȭ
	else
	{
		// �����ӿ� ���� ȸ���� �ٽ� Ȱ��ȭ (���� ����)
		CachedMovementComp->bOrientRotationToMovement = true;

		// �ִ� �̵��ӵ� ���󺹱�
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

		// ��� ���� �±� ����
		UPA_FunctionLibrary::RemoveGameplayTagToActorIfFound(CachedEnemyCharacter.Get(), PA_GameplayTags::Enemy_Status_Boundary);
	}
}
