// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_ComputeAttackChance.h"
#include "Kismet/KismetMathLibrary.h"

UBTDecorator_ComputeAttackChance::UBTDecorator_ComputeAttackChance()
{
	NodeName = "Compute Attack Chance";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	AttackChanceMin = 0.5f;
	AttackChanceMax = 0.5f;
}

bool UBTDecorator_ComputeAttackChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// �ּ� ���� Ȯ��, �ִ� ���� Ȯ�� ���� ���� ���� ��
	const float AttackChance = UKismetMathLibrary::RandomFloatInRange(AttackChanceMin, AttackChanceMax);

	// 0 ~ 1�� ������ ���� ��ȸ ���
	return UKismetMathLibrary::RandomBoolWithWeight(AttackChance);
}
