// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PA_CharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "Components/Combat/PA_PlayerCombatComponent.h"

#include "DataAssets/StartUpData/DA_BaseStartUpData.h"

#include "GameModes/PA_GameModeBase.h"

#include "ProjectATypes/PA_EnumTypes.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"

APA_CharacterPlayer::APA_CharacterPlayer()
{
	GetCapsuleComponent()->InitCapsuleSize(38.f, 82.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GroundFriction = 4.f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-40.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1000.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// ���� ������Ʈ
	PlayerCombatComponent = CreateDefaultSubobject<UPA_PlayerCombatComponent>(TEXT("PlayerCombatComponent"));

	// �ִ� �̵��ӵ� ���� �ݹ� �Լ� ���ε�
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxMovementSpeedAttribute()).AddUObject(this, &APA_CharacterPlayer::OnMaxMovementSpeedChanged);
}

UPA_PawnCombatComponent* APA_CharacterPlayer::GetPawnCombatComponent() const
{
	return PlayerCombatComponent;
}

void APA_CharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

}

void APA_CharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDA_BaseStartUpData* BaseStartUpData = CharacterStartUpData.LoadSynchronous())
		{
			// ���� �� �ο� �� Ȱ��ȭ(OnGive) �����Ƽ �ο�
			GiveStartUpAbilities(BaseStartUpData->ActivateOnGiveAbilities);

			// ���� �� �ο�(OnTriggered) �����Ƽ �ο�
			GiveStartUpAbilities(BaseStartUpData->ReactiveAbilities);

			// �⺻ ĳ���� ���� �����÷��� ����Ʈ ����
			ApplyStartUpEffects(BaseStartUpData->StartUpEffects);
		}
	}
}

void APA_CharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// �̵�
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APA_CharacterPlayer::OnMove);
		}

		// ī�޶� ȸ��
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APA_CharacterPlayer::OnLook);
		}

		// ī�޶� �� ��-�ƿ�
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(CameraZoomAction, ETriggerEvent::Triggered, this, &APA_CharacterPlayer::OnCameraZoom);
		}

		// �޸��� �����Ƽ
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APA_CharacterPlayer::OnSprintStarted);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APA_CharacterPlayer::OnSprintEnded);
		}

		// ���� ���� �����Ƽ
		if (EquipWeaponAction)
		{
			EnhancedInputComponent->BindAction(EquipWeaponAction, ETriggerEvent::Triggered, this, &APA_CharacterPlayer::OnEquipWeapon);
		}
	}
}

void APA_CharacterPlayer::OnMove(const FInputActionValue& InputActionValue)
{
	FVector2D AxisValue = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();

	const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, AxisValue.X);
	AddMovementInput(RightDirection, AxisValue.Y);
}

void APA_CharacterPlayer::OnLook(const FInputActionValue& InputActionValue)
{
	float AxisValue = InputActionValue.Get<float>();
	Controller->SetControlRotation(Controller->GetControlRotation() + FRotator(0.f, AxisValue, 0.f));
}

void APA_CharacterPlayer::OnCameraZoom(const FInputActionValue& InputActionValue)
{
	float AxisValue = -InputActionValue.Get<float>();

	float ZoomValue = FMath::Clamp((SpringArm->TargetArmLength + AxisValue * 500.f), 400.f, 1000.f);
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ZoomValue, GetWorld()->GetDeltaSeconds(), 10.f);
}

void APA_CharacterPlayer::OnSprintStarted(const FInputActionValue& InputActionValue)
{
	AbilitySystemComponent->TryActivateAbilitiesByTag(SprintTags, true);
}

void APA_CharacterPlayer::OnSprintEnded(const FInputActionValue& InputActionValue)
{
	AbilitySystemComponent->CancelAbilities(&SprintTags);
}

void APA_CharacterPlayer::OnEquipWeapon(const FInputActionValue& InputActionValue)
{

}

void APA_CharacterPlayer::GiveStartUpAbilities(const TArray<TSubclassOf<UPA_GameplayAbility>> StartUpAbilties)
{
	for (auto Ability : StartUpAbilties)
	{
		if (Ability)
		{
			FGameplayAbilitySpec AbilitySpec(Ability);
			AbilitySpec.SourceObject = this;
			AbilitySpec.Level = 1;

			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

void APA_CharacterPlayer::ApplyStartUpEffects(const TArray<TSubclassOf<UGameplayEffect>> StartUpEffects)
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (auto StartUpEffect : StartUpEffects)
	{
		ApplyGameplayEffectToSelf(StartUpEffect, EffectContext);
	}
}

void APA_CharacterPlayer::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	// ���޹��� �ִ� �̵��ӵ��� ����
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}
