// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PA_CharacterEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Combat/PA_EnemyCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/PA_BaseWeapon.h"
#include "DataAssets/StartUpData/DA_EnemyStartUpData.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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

	// ���� ������Ʈ
	EnemyCombatComponent = CreateDefaultSubobject<UPA_EnemyCombatComponent>(TEXT("EnemyCombatComponent"));
}

UPA_PawnCombatComponent* APA_CharacterEnemy::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

void APA_CharacterEnemy::OnEnemyDied(float InDuration, float InUpdateInterval, TSoftObjectPtr<class UNiagaraSystem> DissolveNiagara, FLinearColor DissolveNiagaraColor)
{
	// �� ĳ���� ���� ��������
	if (APA_BaseWeapon* Weapon = EnemyCombatComponent->GetCharacterCurrentEquippingWeapon())
	{
		EnemyWeapon = Weapon;
	}

	// �ִϸ��̼� �ߴ�
	GetMesh()->bPauseAnims = true;

	// ��� ��Ƽ���� ȿ�� (������) ���
	UpdateMaterialParameter(InDuration, InUpdateInterval);

	// ��� ������ ���̾ư��� �ý��� �񵿱� �ε�
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		DissolveNiagara.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda([this, DissolveNiagara, DissolveNiagaraColor]()
			{
				// �񵿱� �ε��� ������ ���̾ư��� ����
				UNiagaraSystem* DissolveNiagaraSystem = DissolveNiagara.Get();

				// ������ ���̾ư��� ���� �� ������Ʈ ����
				UNiagaraComponent* DissolveNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
					DissolveNiagaraSystem,
					GetMesh(),
					FName(),
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					EAttachLocation::KeepRelativeOffset,
					true // ���� ���� �� �ڵ� ����
				);

				// �Է¹��� ���̾ư��� ������ �������� ������ ���� ����
				DissolveNiagaraComp->SetNiagaraVariableLinearColor(TEXT("DissolveParticleColor"), DissolveNiagaraColor);
			})
	);
}

void APA_CharacterEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitEnemyStartUpData();
}

void APA_CharacterEnemy::UpdateMaterialParameter(float InDuration, float InUpdateInterval)
{
	// ��� ��Ƽ���� ȿ�� (������) ���
	ElapsedTime = 0.0f;
	Duration = InDuration;
	UpdateInterval = InUpdateInterval;

	// ��� ��Ƽ���� ȿ�� (������) ������Ʈ ���ݸ��� ��Ƽ���� �� ����
	GetWorld()->GetTimerManager().SetTimer(EnemyDiedTimerHandle, this, &APA_CharacterEnemy::UpdateMaterialParameterCallback, InUpdateInterval, true);
}

void APA_CharacterEnemy::UpdateMaterialParameterCallback()
{
	// ������Ʈ ���ݸ�ŭ ��� �ð� �߰�
	ElapsedTime += UpdateInterval;

	// �� ��� �ð����� ���� ����� �ð���ŭ�� ������ ����
	// ��, ���� ��Ƽ���� ��
	float CurrentAlphaValue = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

	// �� ���� ������ ��Ƽ���� �Ķ���� ����
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveAmount"), CurrentAlphaValue);

	// �� ���� ������ ��Ƽ���� �Ķ���� ����
	if (EnemyWeapon)
	{
		if (UStaticMeshComponent* LeftWeapon = EnemyWeapon->GetLeftWeaponMesh())
		{
			LeftWeapon->SetScalarParameterValueOnMaterials(TEXT("DissolveAmount"), CurrentAlphaValue);
		}
		if (UStaticMeshComponent* RightWeapon = EnemyWeapon->GetRightWeaponMesh())
		{
			RightWeapon->SetScalarParameterValueOnMaterials(TEXT("DissolveAmount"), CurrentAlphaValue);
		}
	}

	// ��Ƽ���� ���� 1�� ������ ���, ��� ó�� ����
	if (CurrentAlphaValue >= 1.0f)
	{
		// Ÿ�̸� �ʱ�ȭ
		GetWorld()->GetTimerManager().ClearTimer(EnemyDiedTimerHandle);

		// ���� ���� ����
		if (EnemyWeapon)
		{
			EnemyWeapon->Destroy();
		}

		// �� ���� ����
		Destroy();
	}
}

void APA_CharacterEnemy::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull()) return;

	// �⺻ ���� ������ �񵿱� �ε�
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
