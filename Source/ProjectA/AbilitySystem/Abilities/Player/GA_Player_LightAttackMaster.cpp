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

	// �޺� Ÿ�̸� �ʱ�ȭ
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	// ���� �޺��� ���� ��Ÿ�� Ž��
	UAnimMontage* PlayMontage = *ComboAttackMontagesMap.Find(CurrentCombo);
	if (!PlayMontage) return;

	// ��Ÿ�� �÷��� �½�ũ �ʱ�ȭ
	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, PlayMontage);
	if (MontageTask)
	{
		// ��Ÿ�� ȥ��, �Ϸ�, �ߴ� ��ҿ� EndAbility ���ε�
		MontageTask->OnBlendOut.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);

		// ��Ÿ�� �÷��� �½�ũ Ȱ��ȭ
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

		// ���� ����
		const FVector AttackDirection = GetPlayerCharacterFromActorInfo()->GetLastMovementInputVector().GetSafeNormal();

		if (!AttackDirection.IsNearlyZero())
		{
			// �Է� ���Ϳ� ���� ȸ���� ���
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
