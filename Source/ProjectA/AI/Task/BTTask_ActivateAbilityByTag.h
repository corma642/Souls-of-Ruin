// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Task/BTTask_BaseEnemy.h"
#include "BTTask_ActivateAbilityByTag.generated.h"

UCLASS()
class PROJECTA_API UBTTask_ActivateAbilityByTag : public UBTTask_BaseEnemy
{
	GENERATED_BODY()
	
public:
	UBTTask_ActivateAbilityByTag();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// 태스크 실행 함수 재정의
	virtual void OnExecuteTask() override;

private:
	// 활성화할 어빌리티 태그
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	FGameplayTag AbilityTagToActivate;

};
