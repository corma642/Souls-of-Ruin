// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_ShouldAbortAllLogic.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "PA_FunctionLibrary.h"
#include "PA_GameplayTags.h"

UBTDecorator_ShouldAbortAllLogic::UBTDecorator_ShouldAbortAllLogic()
{
	NodeName = "Should Abort All Logic";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

bool UBTDecorator_ShouldAbortAllLogic::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* OwnerActor = Cast<AActor>(OwnerComp.GetAIOwner()->GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName));
	if (OwnerActor)
	{
		bool bIsTargetActorDead = false;
		bool bIsOwningAIDead = false;

		// Ÿ�� ������ ��� ���� �±� ���� ���� Ȯ��
		bIsTargetActorDead = UPA_FunctionLibrary::NativeDoesActorHaveTag(OwnerActor, PA_GameplayTags::Shared_Status_Dead);

		// AI�� ��� ���� �±� ���� ���� Ȯ��
		bIsOwningAIDead = UPA_FunctionLibrary::NativeDoesActorHaveTag(OwnerComp.GetAIOwner()->GetPawn(), PA_GameplayTags::Shared_Status_Dead);

		// �� �Ǵ� ����(AI) �׾��� ��� true ��ȯ
		return bIsTargetActorDead || bIsOwningAIDead;
	}

	return false;
}
