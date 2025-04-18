// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_GetDistToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_GetDistToTarget::UBTService_GetDistToTarget()
{
	NodeName = "Get Distance To Target";

	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTService_GetDistToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* OwnerController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!OwnerController || !BBComp) return;

	if (AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(InTargetActorKey.SelectedKeyName)))
	{
		float Distance = FVector::Distance(OwnerController->GetPawn()->GetActorLocation(), TargetActor->GetActorLocation());
		BBComp->SetValueAsFloat(OutDistToTargtKey.SelectedKeyName, Distance);
	}
}
