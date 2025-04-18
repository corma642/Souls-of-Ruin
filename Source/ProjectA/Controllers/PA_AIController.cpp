// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/PA_AIController.h"
#include "Characters/PA_CharacterEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Navigation/CrowdFollowingComponent.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

// �⺻ AI ��ã�� �Ŀ� ��ȸ(UCrowdFollowingComponent)�� ����
APA_AIController::APA_AIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	// ���� �Ϲ� �� ID�� 1�� ����
	SetGenericTeamId(FGenericTeamId(1));

	/* UAISenseConfig_Sight ������ ���� �ؾ�  UAIPerceptionComponent ���� ����!!*/
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));

	// �ð� ���� �ý��� ���� [ ������(O) / ģȭ��(X) / �ڿ���(X) ]
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;

	// �þ� ���� ����
	AISenseConfig_Sight->SightRadius = 500.0f; // �þ� �ݰ�
	AISenseConfig_Sight->LoseSightRadius = 1000.0f; // �þ� ��� �ݰ�
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 90.0f; // �ֺ� �þ� ����


	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));

	// ���� ������ �þ� ���� ���� �߰�
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);

	// �ð� ���� ������ �ֿ� �������� ���� (ex. û�� ���� �ٸ� �������� �켱��)
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());

	// AI�� ����� �����ϰų� ���� ���°� ����� ������ �Լ� ȣ�� ���ε�
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &APA_AIController::OnEnemyPerceptionUpdated);
}

ETeamAttitude::Type APA_AIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	// ���� ����� �Ϲ� �� ID �������̽��� ������
	const APawn* OtherPawn = Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherPawnTeamAgent = Cast<const IGenericTeamAgentInterface>(OtherPawn->GetController());

	// ���� ����� �� ID�� ���� �� ID���� ���� ��� ���������� �Ǵ�
	// �÷��̾��� ��� 0, ���� 1
	if (OtherPawnTeamAgent && OtherPawnTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void APA_AIController::BeginPlay()
{
	Super::BeginPlay();

	// AI ��Ʈ�ѷ��� ���� ȸ�� ������Ʈ�� �����´�.
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		// ���� ȸ�� Ȱ��ȭ ���� ����
		CrowdComp->SetCrowdSimulationState(bEnableDefourCrowdAvoidence ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		// ���� ȸ�� ǰ�� ����
		switch (DefourCrowdAvoidenceQulity)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);		break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);	break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);		break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);		break;
		default: break;
		}

		// ȸ�� �׷� ����
		// ���� �׷쿡 ���� AI�鳢�� ���� ȸ���ϵ��� ����
		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);

		// �ݸ��� ���� ���� ����
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void APA_AIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// �����̺�� Ʈ�� ����
	if (EnemyBehaviorTree)
	{
		RunBehaviorTree(EnemyBehaviorTree);

		// �� ĳ������ �⺻ �ִ� �̵��ӵ� ����
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
		// TargetActor�� ������ ������ ������� ����
		if (!BBComp->GetValueAsObject(TEXT("TargetActor")))
		{
			if (Stimuls.WasSuccessfullySensed() && Actor)
			{
				BBComp->SetValueAsObject(TEXT("TargetActor"), Actor);
			}
		}
	}
}
