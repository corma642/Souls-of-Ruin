// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_AttackTargetUpdate.h"
#include "Characters/PA_CharacterEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_AttackTargetUpdate::UBTService_AttackTargetUpdate()
{
	NodeName = "Attack Target Update";

	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_AttackTargetUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!OwningPawn || !BBComp) return;

	// �� ĳ���� ĳ�� �����Ͱ� ��ȿ���� ���� ���
	if (!CachedEnemyCharacter.IsValid())
	{
		// ĳ������ ���� ù ĳ�� ������ ����
		CachedEnemyCharacter = Cast<APA_CharacterEnemy>(OwningPawn);
	}
	check(CachedEnemyCharacter.IsValid());
}
