// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_LookAtTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_LookAtTarget::UBTService_LookAtTarget()
{
	// 비헤이비어 트리에서 보일 서비스 이름
	NodeName = TEXT("Native Look At Target");

	// / 필수 추가 / 기본 서비스가 제대로 동작하도록 플래그를 알리는 매크로
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	// 블랙보드 키를 초기화
	// InTargetActorKey는 AActor 타입의 객체를 필터링하도록 설정
	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());

	// 각 변수값들 초기화
	RotationInterpSpeed = 3.f;
	Interval = 0.f;
	RandomDeviation = 0.f;
}

void UBTService_LookAtTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 공격 대상을 가져옴
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(InTargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetObject);

	// 서비스를 수행하는 AI 소유자를 가져옴
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

	// 비헤이비어 트리의 블랙보드 에셋 데이터를 가져온다
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		// 블랙보드 키 셀렉터에서 블랙보드 데이터에 접근할 수 있도록 함
		InTargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UBTService_LookAtTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}
