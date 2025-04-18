// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_BaseEnemy.generated.h"

UCLASS()
class PROJECTA_API UBTTask_BaseEnemy : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 태스크 실행 함수
	virtual void OnExecuteTask();

	// 적 캐릭터 캐시 저장
	UPROPERTY()
	TWeakObjectPtr<class APA_CharacterEnemy> CachedEnemyCharacter;
};
