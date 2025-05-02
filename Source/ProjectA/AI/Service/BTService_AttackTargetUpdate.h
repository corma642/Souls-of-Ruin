// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AttackTargetUpdate.generated.h"

UCLASS()
class PROJECTA_API UBTService_AttackTargetUpdate : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_AttackTargetUpdate();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	// 타깃 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "Custom | Service")
	FBlackboardKeySelector InTargetActorKey;

	// 적 캐릭터 캐시 저장
	UPROPERTY()
	TWeakObjectPtr<class APA_CharacterEnemy> CachedEnemyCharacter;

};
