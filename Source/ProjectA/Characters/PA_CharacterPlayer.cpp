// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PA_CharacterPlayer.h"

#include "Camera/CameraComponent.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "DataAssets/StartUpData/DA_BaseStartUpData.h"

#include "GameModes/PA_GameModeBase.h"

#include "ProjectATypes/PA_EnumTypes.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"

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
			int32 AbilityApplyLevel = 1;

			if (APA_GameModeBase* BaseGameMode = GetWorld()->GetAuthGameMode<APA_GameModeBase>())
			{
				switch (BaseGameMode->GetCurrentGameDifficulty())
				{
				case EPA_GameDifficulty::Easy:		// 쉬움
					AbilityApplyLevel = 4; break;

				case EPA_GameDifficulty::Normal:	// 보통
					AbilityApplyLevel = 3; break;

				case EPA_GameDifficulty::Hard:		// 어려움
					AbilityApplyLevel = 2; break;

				case EPA_GameDifficulty::VeryHard:	// 매우 어려움
					AbilityApplyLevel = 1; break;

				default: break;
				}
			}

			// 기본 캐릭터 시작 어빌리티 부여
			GiveStartUpAbilities(BaseStartUpData->StartUpAbilties);

			// 기본 캐릭터 시작 게임플레이 이펙트 적용
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
		// 이동
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APA_CharacterPlayer::OnMove);
	
		// 카메라 이동
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APA_CharacterPlayer::OnLook);
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

void APA_CharacterPlayer::GiveStartUpAbilities(const TArray<TSubclassOf<UGameplayAbility>> StartUpAbilties)
{
	for (auto StartUpAbility : StartUpAbilties)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartUpAbility));
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
