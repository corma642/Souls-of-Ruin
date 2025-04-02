// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/GA_Player_LightAttackMaster.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGA_Player_LightAttackMaster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CurrentCombo > ComboAttackMontagesMap.Num())
	{
		CurrentCombo = 1;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 콤보 타이머 초기화
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	// 현재 콤보의 공격 몽타주 탐색
	UAnimMontage* PlayMontage = *ComboAttackMontagesMap.Find(CurrentCombo);
	if (!PlayMontage) return;

	// 몽타주 플레이 태스크 초기화
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, PlayMontage);
	if (MontageTask)
	{
		// 몽타주 혼합, 완료, 중단 취소에 EndAbility 바인딩
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);

		// 몽타주 플레이 태스크 활성화
		MontageTask->ReadyForActivation();
	}

	bComboInputReceived = true;
}

void UGA_Player_LightAttackMaster::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetComboAttack();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Player_LightAttackMaster::OnComboNotifyReceived(const FGameplayEventData Payload)
{
	if (bComboInputReceived)
	{
		CurrentCombo++;

		// 공격 방향
		const FVector AttackDirection = GetPlayerCharacterFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();

		if (!AttackDirection.IsNearlyZero())
		{
			// 입력 벡터에 따른 회전값 계산
			const FRotator NewRotation = AttackDirection.Rotation();
			GetPlayerCharacterFromActorInfo()->SetActorRotation(NewRotation);
		}

		ActivateAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
	bComboInputReceived = false;
}

void UGA_Player_LightAttackMaster::ResetComboAttack()
{
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			CurrentCombo = 1;
			bComboInputReceived = false;
		}), 0.5f, false);
}
