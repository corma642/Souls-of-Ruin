// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PA_CharacterBase.h"

#include "AbilitySystem/PA_AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"

#include "DataAssets/StartUpData/DA_BaseStartUpData.h"

APA_CharacterBase::APA_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	AbilitySystemComponent = CreateDefaultSubobject<UPA_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	AttributeSet = CreateDefaultSubobject<UPA_AttributeSetBase>(TEXT("AttributeSet"));
}

void APA_CharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

UPA_PawnCombatComponent* APA_CharacterBase::GetPawnCombatComponent() const
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

bool APA_CharacterBase::ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContextHandle)
{
	if (!Effect.Get()) return false;

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, InEffectContextHandle);
	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		return ActiveGEHandle.WasSuccessfullyApplied();
	}

	return false;
}

void APA_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void APA_CharacterBase::GiveStartUpAbilities(const TArray<TSubclassOf<UPA_GameplayAbility>> StartUpAbilties, int32 ApplyLevel)
{
}

void APA_CharacterBase::ApplyStartUpEffects(const TArray<TSubclassOf<UGameplayEffect>> StartUpEffects)
{
}
