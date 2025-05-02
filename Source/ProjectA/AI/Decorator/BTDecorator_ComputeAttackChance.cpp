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
	// 최소 공격 확률, 최대 공격 확률 범위 내의 랜덤 값
	const float AttackChance = UKismetMathLibrary::RandomFloatInRange(AttackChanceMin, AttackChanceMax);

	// 0 ~ 1의 값으로 공격 기회 계산
	return UKismetMathLibrary::RandomBoolWithWeight(AttackChance);
}
