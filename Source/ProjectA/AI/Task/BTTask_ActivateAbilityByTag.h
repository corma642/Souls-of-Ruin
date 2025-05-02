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
	// �½�ũ ���� �Լ� ������
	virtual void OnExecuteTask() override;

private:
	// Ȱ��ȭ�� �����Ƽ �±�
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	FGameplayTag AbilityTagToActivate;

};
