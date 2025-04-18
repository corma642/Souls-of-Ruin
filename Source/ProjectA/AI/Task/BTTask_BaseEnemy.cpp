// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_BaseEnemy.h"
#include "Characters/PA_CharacterEnemy.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_BaseEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* OwningPawn = AIController->GetPawn();
	if (!AIController || !OwningPawn) return EBTNodeResult::Failed;

	// �� ĳ���� ĳ�� �����Ͱ� ��ȿ���� ���� ���
	if (!CachedEnemyCharacter.IsValid())
	{
		// ĳ������ ���� ù ĳ�� ������ ����
		CachedEnemyCharacter = Cast<APA_CharacterEnemy>(OwningPawn);
		
		// ĳ������ �������� �ұ��ϰ� ��ȿ���� ���� ���
		if (!CachedEnemyCharacter.IsValid())
		{
			// �½�ũ�� ���з� �Ϸ�
			FinishExecute(false);
			return EBTNodeResult::Failed;
		}
	}

	// �½�ũ ���� �Լ��� ȣ�� (�½�ũ ���� ����)
	OnExecuteTask();

	// �½�ũ�� �������� �Ϸ�
	FinishExecute(true);
	return EBTNodeResult::Succeeded;
}

void UBTTask_BaseEnemy::OnExecuteTask()
{
}
