// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PA_CharacterEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/PA_EnemyCombatComponent.h"

//#include "DataAssets/StartUpData/DA_BaseStartUpData.h"
#include "DataAssets/StartUpData/DA_EnemyStartUpData.h"

#include "Engine/AssetManager.h"

APA_CharacterEnemy::APA_CharacterEnemy()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	// 전투 컴포넌트
	EnemyCombatComponent = CreateDefaultSubobject<UPA_EnemyCombatComponent>(TEXT("EnemyCombatComponent"));
}

UPA_PawnCombatComponent* APA_CharacterEnemy::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

void APA_CharacterEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void APA_CharacterEnemy::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull()) return;

	// 기본 시작 데이터 비동기 로딩
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[&]()
			{
				if (UDA_EnemyStartUpData* LoadedData = Cast<UDA_EnemyStartUpData>(CharacterStartUpData.Get()))
				{
					LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
				}
			}
		)
	);
}
