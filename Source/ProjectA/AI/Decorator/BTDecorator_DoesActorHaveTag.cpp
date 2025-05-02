// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/BTDecorator_DoesActorHaveTag.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "PA_FunctionLibrary.h"
#include "PA_GameplayTags.h"

UBTDecorator_DoesActorHaveTag::UBTDecorator_DoesActorHaveTag()
{
	NodeName = "Does Actor Have Tag";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();

	InverseConditionCheck = false;
}

bool UBTDecorator_DoesActorHaveTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* OwnerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(InActorKeyToCheck.SelectedKeyName));
	bool Result = false;

	if (OwnerActor)
	{
		// 태그가 있는 경우
		if (UPA_FunctionLibrary::NativeDoesActorHaveTag(OwnerActor, TagToCheck))
		{
			InverseConditionCheck ? Result = false : Result = true;
		}
		// 태그가 없는 경우
		else
		{
			InverseConditionCheck ? Result = true : Result = false;
		}
	}

	return Result;
}
