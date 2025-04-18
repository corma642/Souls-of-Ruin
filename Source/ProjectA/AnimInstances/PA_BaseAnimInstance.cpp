// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/PA_BaseAnimInstance.h"
#include "Characters/PA_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

#include "PA_FunctionLibrary.h"
#include "PA_GameplayTags.h"

void UPA_BaseAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<APA_CharacterBase>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		MovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UPA_BaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (OwningCharacter && MovementComponent)
	{
		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();

		Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, OwningCharacter->GetActorRotation());

		bShoudMove = (MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector) && (GroundSpeed > 3.f);
	
		bIsFalling = MovementComponent->IsFalling();
	}
}

bool UPA_BaseAnimInstance::DoesOwnerHaveTag(const FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UPA_FunctionLibrary::NativeDoesActorHaveTag(OwningPawn, TagToCheck);
	}
	return false;
}
