// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/BTTask_ActivateAbilityByTag.h"
#include "Characters/PA_CharacterEnemy.h"
#include "AbilitySystem/PA_AbilitySystemComponent.h"

UBTTask_ActivateAbilityByTag::UBTTask_ActivateAbilityByTag()
{
	NodeName = "Activate Ability By Tag";

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_ActivateAbilityByTag::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UBTTask_ActivateAbilityByTag::OnExecuteTask()
{
	check(CachedEnemyCharacter.IsValid());

	// 태그를 통해 어빌리티 활성화
	Cast<UPA_AbilitySystemComponent>(CachedEnemyCharacter->GetAbilitySystemComponent())->TryActivateAbilityByTag(AbilityTagToActivate);
}
