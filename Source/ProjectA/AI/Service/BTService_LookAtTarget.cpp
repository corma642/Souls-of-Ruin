// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_LookAtTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_LookAtTarget::UBTService_LookAtTarget()
{
	NodeName = "Look At Target";

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	RotationInterpSpeed = 3.0f;
}

void UBTService_LookAtTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!OwningPawn || !BBComp) return;

	if (AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(InTargetActorKey.SelectedKeyName)))
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		LookAtRot.Pitch = OwningPawn->GetActorRotation().Pitch;
		LookAtRot.Roll = OwningPawn->GetActorRotation().Roll;
		
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);
	
		OwningPawn->SetActorRotation(TargetRot);
	}
}
