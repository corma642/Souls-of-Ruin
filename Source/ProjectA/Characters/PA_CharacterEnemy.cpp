// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PA_CharacterEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Weapons/PA_BaseWeapon.h"
#include "DataAssets/StartUpData/DA_EnemyStartUpData.h"

#include "Components/Combat/PA_EnemyCombatComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/UI/PA_EnemyUIComponent.h"
#include "Components/WidgetComponent.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Widgets/PA_BaseWidget.h"

#include "Controllers/PA_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	// ���� ������Ʈ
	EnemyCombatComponent = CreateDefaultSubobject<UPA_EnemyCombatComponent>(TEXT("EnemyCombatComponent"));

	// UI ������Ʈ
	EnemyUIComponent = CreateDefaultSubobject<UPA_EnemyUIComponent>(TEXT("EnemyUIComponent"));

	// ü�� �� ���� ������Ʈ
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());

	// �ִ� �̵��ӵ� ���� �ݹ� �Լ� ���ε�
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxMovementSpeedAttribute()).AddUObject(this, &APA_CharacterEnemy::OnMaxMovementSpeedChanged);
}

UPA_PawnCombatComponent* APA_CharacterEnemy::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPA_PawnUIComponent* APA_CharacterEnemy::GetUIComponent() const
{
	return EnemyUIComponent;
}

UPA_EnemyUIComponent* APA_CharacterEnemy::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void APA_CharacterEnemy::OnEnemyDied(float InDuration, float InUpdateInterval, TSoftObjectPtr<class UNiagaraSystem> DissolveNiagara, FLinearColor DissolveNiagaraColor)
{
	// �� ĳ���� ���� ��������
	if (EnemyCombatComponent)
	{
		if (APA_BaseWeapon* Weapon = EnemyCombatComponent->GetCharacterCurrentEquippingWeapon())
		{
			EnemyWeapon = Weapon;
		}
	}

	// �ִϸ��̼� �ߴ�
	GetMesh()->bPauseAnims = true;

	// ��� ��Ƽ���� ȿ�� (������) ���
	UpdateMaterialParameter(InDuration, InUpdateInterval);

	// ���Ͱ� �Ҹ�� �� �񵿱� �۾��� ���� ���� �� �����Ƿ� �� ���� ���
	TWeakObjectPtr<APA_CharacterEnemy> WeakTHis = this;

	// ��� ������ ���̾ư��� �ý��� �񵿱� �ε�
	if (!DissolveNiagara.IsNull())
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			DissolveNiagara.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([WeakTHis, DissolveNiagara, DissolveNiagaraColor]()
				{
					if (WeakTHis.IsValid())
					{
						// �񵿱� �ε��� ������ ���̾ư��� ����
						UNiagaraSystem* DissolveNiagaraSystem = DissolveNiagara.Get();

						// ������ ���̾ư��� ���� �� ������Ʈ ����
						UNiagaraComponent* DissolveNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
							DissolveNiagaraSystem,
							WeakTHis->GetMesh(),
							FName(),
							FVector::ZeroVector,
							FRotator::ZeroRotator,
							EAttachLocation::KeepRelativeOffset,
							true // ���� ���� �� �ڵ� ����
						);

						// �Է¹��� ���̾ư��� ������ �������� ������ ���� ����
						if (DissolveNiagaraComp)
						{
							DissolveNiagaraComp->SetNiagaraVariableLinearColor(TEXT("DissolveParticleColor"), DissolveNiagaraColor);
						}
					}
				})
		);
	}
}

void APA_CharacterEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (UPA_BaseWidget* HealthWidget = Cast<UPA_BaseWidget>(EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		// ���� �ʱ�ȭ
		HealthWidget->InitEnemyCreatedWidget(this);
	}
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
			[this]()
			{
				if (UDA_EnemyStartUpData* LoadedData = Cast<UDA_EnemyStartUpData>(CharacterStartUpData.Get()))
				{
					LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
				}
			}
		)
	);
}

void APA_CharacterEnemy::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	// ���޹��� �ִ� �̵��ӵ��� ����
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}
