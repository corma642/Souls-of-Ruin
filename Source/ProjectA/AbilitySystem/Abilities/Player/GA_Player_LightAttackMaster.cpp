// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/GA_Player_LightAttackMaster.h"
#include "Characters/PA_CharacterPlayer.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGA_Player_LightAttackMaster::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// �޺� Ÿ�̸� �ʱ�ȭ
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);

	// ���� �޺��� ���� ��Ÿ�� Ž��
	UAnimMontage* PlayMontage = *AttackMontageMap.Find(CurrenCombo);
	if (!PlayMontage) return;

	MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, PlayMontage);

	// ��Ÿ�ְ� ȥ��, �Ϸ�, �ߴ�, ��ҵǾ��� ��, EndAbility ȣ��
	MontageTask->OnBlendOut.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
	MontageTask->OnCompleted.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
	MontageTask->OnInterrupted.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);
	MontageTask->OnCancelled.AddDynamic(this, &UGA_Player_LightAttackMaster::K2_EndAbility);

	// ��Ÿ�� �½�ũ Ȱ��ȭ
	MontageTask->ReadyForActivation();

	bComboAttack = true;
}

void UGA_Player_LightAttackMaster::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	ResetComboAttackCount();
	bComboAttack = false;
}

void UGA_Player_LightAttackMaster::NextComboAttack()
{
	if (bComboAttack)
	{
		// ���� �޺��� �ִ� �޺��� �ƴ� ���
		if (CurrenCombo < AttackMontageMap.Num())
		{
			// �޺� ī��Ʈ ����
			CurrenCombo++;
		}
		else
		{
			// �޺� �ʱ�ȭ
			ResetComboAttackCount();
		}
	}
	else
	{
		// �޺� �ʱ�ȭ
		ResetComboAttackCount();
	}
}

void UGA_Player_LightAttackMaster::ResetComboAttackCount()
{
	GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			CurrenCombo = 1;
		}), 0.3f, false);
}
