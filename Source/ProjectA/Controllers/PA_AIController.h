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
	// ���� ��� ���� �µ�(�� ID)
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	/* IGenericTeamAgentInterface */

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	// ������ ��� ���� �� ���� ���� ���� �� ȣ�� ��������Ʈ �̺�Ʈ �Լ�
	UFUNCTION()
	void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimuls);

protected:
	// AI �þ� ���� ���� ó�� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | AI")
	TObjectPtr<class UAISenseConfig_Sight> AISenseConfig_Sight;

	// AI �ν� ó�� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | AI")
	TObjectPtr<class UAIPerceptionComponent> EnemyPerceptionComponent;

	// �����̺�� Ʈ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AI")
	TObjectPtr<class UBehaviorTree> EnemyBehaviorTree;

private:
	// ���� ��ȸ ȸ�� Ȱ��ȭ ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Defour Crowd Avoidence", meta = (AllowPrivateAccess = "true"))
	bool bEnableDefourCrowdAvoidence = true;

	// ���� ��ȸ ȸ�� ǰ�� ���� / �ּ�(1) / �ִ�(4)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Defour Crowd Avoidence", meta = (EditCondition = "bEnableDefourCrowdAvoidence", UIMin = "1", UIMax = "4", AllowPrivateAccess = "true"))
	int32 DefourCrowdAvoidenceQulity = 4;

	// �ݸ��� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Defour Crowd Avoidence", meta = (EditCondition = "bEnableDefourCrowdAvoidence", AllowPrivateAccess = "true"))
	float CollisionQueryRange = 600.0f;
};
