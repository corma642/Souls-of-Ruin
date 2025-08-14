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

	// 전투 컴포넌트
	EnemyCombatComponent = CreateDefaultSubobject<UPA_EnemyCombatComponent>(TEXT("EnemyCombatComponent"));

	// UI 컴포넌트
	EnemyUIComponent = CreateDefaultSubobject<UPA_EnemyUIComponent>(TEXT("EnemyUIComponent"));

	// 체력 바 위젯 컴포넌트
	EnemyHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("EnemyHealthWidgetComponent"));
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());

	// 최대 이동속도 변경 콜백 함수 바인딩
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
	// 적 캐릭터 무기 가져오기
	if (EnemyCombatComponent)
	{
		if (APA_BaseWeapon* Weapon = EnemyCombatComponent->GetCharacterCurrentEquippingWeapon())
		{
			EnemyWeapon = Weapon;
		}
	}

	// 애니메이션 중단
	GetMesh()->bPauseAnims = true;

	// 사망 머티리얼 효과 (디졸브) 재생
	UpdateMaterialParameter(InDuration, InUpdateInterval);

	// 액터가 소멸될 때 비동기 작업이 진행 중일 수 있으므로 약 참조 사용
	TWeakObjectPtr<APA_CharacterEnemy> WeakTHis = this;

	// 사망 디졸브 나이아가라 시스템 비동기 로딩
	if (!DissolveNiagara.IsNull())
	{
		UAssetManager::GetStreamableManager().RequestAsyncLoad(
			DissolveNiagara.ToSoftObjectPath(),
			FStreamableDelegate::CreateLambda([WeakTHis, DissolveNiagara, DissolveNiagaraColor]()
				{
					if (WeakTHis.IsValid())
					{
						// 비동기 로딩된 디졸브 나이아가라 저장
						UNiagaraSystem* DissolveNiagaraSystem = DissolveNiagara.Get();

						// 디졸브 나이아가라 생성 후 컴포넌트 저장
						UNiagaraComponent* DissolveNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
							DissolveNiagaraSystem,
							WeakTHis->GetMesh(),
							FName(),
							FVector::ZeroVector,
							FRotator::ZeroRotator,
							EAttachLocation::KeepRelativeOffset,
							true // 액터 제거 시 자동 제거
						);

						// 입력받은 나이아가라 디졸브 색상으로 디졸브 색상 변경
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
		// 위젯 초기화
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
	// 사망 머티리얼 효과 (디졸브) 재생
	ElapsedTime = 0.0f;
	Duration = InDuration;
	UpdateInterval = InUpdateInterval;

	// 사망 머티리얼 효과 (디졸브) 업데이트 간격마다 머티리얼 값 갱신
	GetWorld()->GetTimerManager().SetTimer(EnemyDiedTimerHandle, this, &APA_CharacterEnemy::UpdateMaterialParameterCallback, InUpdateInterval, true);
}

void APA_CharacterEnemy::UpdateMaterialParameterCallback()
{
	// 업데이트 간격만큼 경과 시간 추가
	ElapsedTime += UpdateInterval;

	// 총 재생 시간에서 현재 경과한 시간만큼의 비율을 구함
	// 즉, 현재 머티리얼 값
	float CurrentAlphaValue = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);

	// 적 액터 디졸브 머티리얼 파라미터 갱신
	GetMesh()->SetScalarParameterValueOnMaterials(TEXT("DissolveAmount"), CurrentAlphaValue);

	// 적 무기 디졸브 머티리얼 파라미터 갱신
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

	// 머티리얼 값이 1에 도달한 경우, 사망 처리 진행
	if (CurrentAlphaValue >= 1.0f)
	{
		// 타이머 초기화
		GetWorld()->GetTimerManager().ClearTimer(EnemyDiedTimerHandle);

		// 무기 액터 제거
		if (EnemyWeapon)
		{
			EnemyWeapon->Destroy();
		}

		// 적 액터 제거
		Destroy();
	}
}

void APA_CharacterEnemy::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull()) return;

	// 기본 시작 데이터 비동기 로딩
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
	// 전달받은 최대 이동속도로 갱신
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}
