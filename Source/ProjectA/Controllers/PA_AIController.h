// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PA_AIController.generated.h"

UCLASS()
class PROJECTA_API APA_AIController : public AAIController
{
	GENERATED_BODY()

public:
	APA_AIController(const FObjectInitializer& ObjectInitializer);

	/* IGenericTeamAgentInterface */
	// 감지 대상에 대한 태도(팀 ID)
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	/* IGenericTeamAgentInterface */

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// 적대적 대상 감지 및 감지 상태 변경 시 호출 델리게이트 이벤트 함수
	UFUNCTION()
	void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimuls);

protected:
	// AI 시야 감각 정보 처리 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | AI")
	TObjectPtr<class UAISenseConfig_Sight> AISenseConfig_Sight;

	// AI 인식 처리 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | AI")
	TObjectPtr<class UAIPerceptionComponent> EnemyPerceptionComponent;

	// 비헤이비어 트리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AI")
	TObjectPtr<class UBehaviorTree> EnemyBehaviorTree;

private:
	// 군중 우회 회피 활성화 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Defour Crowd Avoidence", meta = (AllowPrivateAccess = "true"))
	bool bEnableDefourCrowdAvoidence = true;

	// 군중 우회 회피 품질 설정 / 최소(1) / 최대(4)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Defour Crowd Avoidence", meta = (EditCondition = "bEnableDefourCrowdAvoidence", UIMin = "1", UIMax = "4", AllowPrivateAccess = "true"))
	int32 DefourCrowdAvoidenceQulity = 4;

	// 콜리전 쿼리 벙뮈
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Defour Crowd Avoidence", meta = (EditCondition = "bEnableDefourCrowdAvoidence", AllowPrivateAccess = "true"))
	float CollisionQueryRange = 600.0f;
};
