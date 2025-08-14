// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_LookAtTarget.generated.h"

UCLASS()
class PROJECTA_API UBTService_LookAtTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_LookAtTarget();

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// �����忡 �ִ� �����Ϳ� ������ �� �ֵ��� �ʱ�ȭ�ϴ� �Լ�
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	// �� ������ ���� ������ ��ȯ�ϴ� �Լ�
	virtual FString GetStaticDescription() const override;

private:
	// Ÿ�� ������ Ű
	UPROPERTY(EditAnywhere, Category = "Cusstom | Service")
	FBlackboardKeySelector InTargetActorKey;

	// ȸ�� ��, ȸ�� ���� ��
	UPROPERTY(EditAnywhere, Category = "Cusstom | Service")
	float RotationInterpSpeed;
};
