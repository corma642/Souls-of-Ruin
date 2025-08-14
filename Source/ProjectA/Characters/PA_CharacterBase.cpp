// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PA_CharacterBase.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"

#include "MotionWarpingComponent.h"

#include "DataAssets/StartUpData/DA_BaseStartUpData.h"

APA_CharacterBase::APA_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	AbilitySystemComponent = CreateDefaultSubobject<UPA_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UPA_AttributeSetBase>(TEXT("AttributeSet"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void APA_CharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

UPA_PawnCombatComponent* APA_CharacterBase::GetPawnCombatComponent() const
{
	return nullptr;
}

UPA_PawnUIComponent* APA_CharacterBase::GetUIComponent() const
{
	return nullptr;
}

void APA_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

}

UAbilitySystemComponent* APA_CharacterBase::GetAbilitySystemComponent() const
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent;
	}
	return nullptr;
}

void APA_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void APA_CharacterBase::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
}

void APA_CharacterBase::SetMaxMovementSpeedAndAttribute(float NewSpeed)
{
	AbilitySystemComponent->SetNumericAttributeBase(AttributeSet->GetMaxMovementSpeedAttribute(), NewSpeed);
}
