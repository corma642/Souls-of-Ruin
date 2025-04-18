// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Task/BTTask_BaseEnemy.h"
#include "BTTask_ToggleBoundaryState.generated.h"

UCLASS()
class PROJECTA_API UBTTask_ToggleBoundaryState : public UBTTask_BaseEnemy
{
	GENERATED_BODY()
	
public:
	UBTTask_ToggleBoundaryState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	// �½�ũ ���� �Լ� ������
	virtual void OnExecuteTask() override;

private:
	// ��� ���� ����
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	bool bShouldEnable;

	// ��� ���� �ִ� �̵��ӵ� ���� ����
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	bool bShouldChangeMaxWalkSpeed;

	// ��� ���� �ִ� �̵��ӵ�
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	float BoundaryWalkSpeed;

	// �⺻ �ִ� �̵��ӵ� ������ Ű
	UPROPERTY(EditAnywhere, Category = "Custom | Task")
	FBlackboardKeySelector InOriginalMaxWalkSpeedKey;

	// �� ĳ���� �����Ʈ ������Ʈ ĳ��
	TWeakObjectPtr<class UCharacterMovementComponent> CachedMovementComp;
};
