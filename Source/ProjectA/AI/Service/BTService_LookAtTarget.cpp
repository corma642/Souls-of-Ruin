// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_LookAtTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_LookAtTarget::UBTService_LookAtTarget()
{
	// �����̺�� Ʈ������ ���� ���� �̸�
	NodeName = TEXT("Native Look At Target");

	// / �ʼ� �߰� / �⺻ ���񽺰� ����� �����ϵ��� �÷��׸� �˸��� ��ũ��
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	// ������ Ű�� �ʱ�ȭ
	// InTargetActorKey�� AActor Ÿ���� ��ü�� ���͸��ϵ��� ����
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());

	// �� �������� �ʱ�ȭ
	RotationInterpSpeed = 3.f;
	Interval = 0.f;
	RandomDeviation = 0.f;
}

void UBTService_LookAtTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// ���� ����� ������
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	// ���񽺸� �����ϴ� AI �����ڸ� ������
	APawn* OwningPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());

	if (OwningPawn && TargetActor)
	{
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(), TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds, RotationInterpSpeed);

		OwningPawn->SetActorRotation(TargetRot);
	}
}

void UBTService_LookAtTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	// �����̺�� Ʈ���� ������ ���� �����͸� �����´�
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		// ������ Ű �����Ϳ��� ������ �����Ϳ� ������ �� �ֵ��� ��
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTService_LookAtTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}
