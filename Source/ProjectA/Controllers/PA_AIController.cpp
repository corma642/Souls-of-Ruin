// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/PA_AIController.h"
#include "Characters/PA_CharacterEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

// 기본 AI 길찾기 궂웅 우회(UCrowdFollowingComponent)로 설정
APA_AIController::APA_AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	// 적의 일반 팀 ID를 1로 설정
	SetGenericTeamId(FGenericTeamId(1));

	/* UAISenseConfig_Sight 설정을 먼저 해야  UAIPerceptionComponent 설정 가능!!*/
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));

	// 시각 감지 시스템 설정 [ 적대적(O) / 친화적(X) / 자연적(X) ]
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;

	// 시야 범위 설정
	AISenseConfig_Sight->SightRadius = 500.0f; // 시야 반경
	AISenseConfig_Sight->LoseSightRadius = 1000.0f; // 시야 상실 반경
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 90.0f; // 주변 시야 각도


	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));

	// 감각 정보에 시야 감각 정보 추가
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);

	// 시각 감각 정보를 주요 감각으로 설정 (ex. 청각 같은 다른 감각보다 우선시)
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());

	// AI가 대상을 감지하거나 감지 상태가 변경될 때마다 함수 호출 바인딩
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APA_AIController::OnEnemyPerceptionUpdated);
}

ETeamAttitude::Type APA_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// 감지 대상의 일반 팀 ID 인터페이스를 가져옴
	const APawn* OtherPawn = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherPawnTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController());

	// 감지 대상의 팀 ID가 나의 팀 ID보다 작은 경우 적대적으로 판단
	// 플레이어의 경우 0, 적은 1
	if (OtherPawnTeamAgent && OtherPawnTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void APA_AIController::BeginPlay()
{
	Super::BeginPlay();

	// AI 컨트롤러의 군중 회피 컴포넌트를 가져온다.
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		// 군중 회피 활성화 여부 설정
		CrowdComp->SetCrowdSimulationState(bEnableDefourCrowdAvoidence ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		// 군중 회피 품질 설정
		switch (DefourCrowdAvoidenceQulity)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);		break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);	break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);		break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);		break;
		default: break;
		}

		// 회피 그룹 설정
		// 같은 그룹에 속한 AI들끼리 서로 회피하도록 설정
		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);

		// 콜리전 쿼리 범위 설정
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void APA_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 비헤이비어 트리 수행
	if (EnemyBehaviorTree)
	{
		RunBehaviorTree(EnemyBehaviorTree);

		// 적 캐릭터의 기본 최대 이동속도 저장
		if (APA_CharacterEnemy* Enemy = Cast<APA_CharacterEnemy>(InPawn))
		{
			if (UBlackboardComponent* BBComp = GetBlackboardComponent())
			{
				BBComp->SetValueAsFloat(
					TEXT("OriginalMaxWalkSpeed"),
					Enemy->GetMovementComponent()->GetMaxSpeed()
				);
			}
		}
	}
}

void APA_AIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimuls)
{
	if (UBlackboardComponent* BBComp = GetBlackboardComponent())
	{
		// TargetActor를 감지한 적대적 대상으로 설정
		if (!BBComp->GetValueAsObject(TEXT("TargetActor")))
		{
			if (Stimuls.WasSuccessfullySensed() && Actor)
			{
				BBComp->SetValueAsObject(TEXT("TargetActor"), Actor);
			}
		}
	}
}
