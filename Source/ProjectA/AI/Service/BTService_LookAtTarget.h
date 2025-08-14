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

	// 블랙보드에 있는 데이터에 접근할 수 있도록 초기화하는 함수
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	// 이 서비스의 정적 설명을 반환하는 함수
	virtual FString GetStaticDescription() const override;

private:
	// 타깃 블랙보드 키
	UPROPERTY(EditAnywhere, Category = "Cusstom | Service")
	FBlackboardKeySelector InTargetActorKey;

	// 회전 시, 회전 보간 값
	UPROPERTY(EditAnywhere, Category = "Cusstom | Service")
	float RotationInterpSpeed;
};
