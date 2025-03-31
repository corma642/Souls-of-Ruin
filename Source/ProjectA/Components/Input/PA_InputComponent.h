// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DA_InputConfig.h"
#include "PA_InputComponent.generated.h"

UCLASS()
class PROJECTA_API UPA_InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	// �⺻ �Է� �׼� ���ε� �Լ�
	template<class UserObject, typename CallbackFunc>
	void NativeInputActionBind(const UDA_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	// �����Ƽ �Է� �׼� ���ε� �Լ�
	template<class UserObject, typename CallbackFunc>																						   
	void AbilityInputActionBind(const UDA_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};

// �⺻ �Է� �׼� ���ε� ����
template<class UserObject, typename CallbackFunc>
inline void UPA_InputComponent::NativeInputActionBind(const UDA_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	check(InInputConfig);

	// �Է� ���� ������ ������ �⺻ �Է� �׼� �迭����, �ش� �Է� �±׿� �´� �Է� �׼��� ������
	if (UInputAction* InputAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		// ĳ���Ϳ� �ش� �Է� �׼ǰ� �Լ��� ���ε�
		BindAction(InputAction, TriggerEvent, ContextObject, Func);
	}
}

// �����Ƽ �Է� �׼� ���ε� ����
template<class UserObject, typename CallbackFunc>
inline void UPA_InputComponent::AbilityInputActionBind(const UDA_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	check(InInputConfig);

	// �Է� ���� ������ ������ �����Ƽ �Է� �׼� �迭�� ��ȸ
	for (const FInputActionConfig& AbilityInputAction : InInputConfig->AbilityInputActions)
	{
		// �ش� �Է� �׼� ������ ��ȿ���� ������ �н�
		if (!AbilityInputAction.IsValid()) continue;

		// ĳ���Ϳ� �ش� �Է� �׼��� ����(Started) �̺�Ʈ�� �Լ��� ���ε�, �Է� �±׸� ����
		BindAction(AbilityInputAction.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputAction.InputTag);
		
		// ĳ���Ϳ� �ش� �Է� �׼��� ����(Completed) �̺�Ʈ�� �Լ��� ���ε�, �Է� �±׸� ����
		BindAction(AbilityInputAction.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputAction.InputTag);
	}
}