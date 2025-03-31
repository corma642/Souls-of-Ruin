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
	// 기본 입력 액션 바인딩 함수
	template<class UserObject, typename CallbackFunc>
	void NativeInputActionBind(const UDA_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func);

	// 어빌리티 입력 액션 바인딩 함수
	template<class UserObject, typename CallbackFunc>																						   
	void AbilityInputActionBind(const UDA_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc);
};

// 기본 입력 액션 바인딩 구현
template<class UserObject, typename CallbackFunc>
inline void UPA_InputComponent::NativeInputActionBind(const UDA_InputConfig* InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent, UserObject* ContextObject, CallbackFunc Func)
{
	check(InInputConfig);

	// 입력 구성 데이터 에셋의 기본 입력 액션 배열에서, 해당 입력 태그에 맞는 입력 액션을 가져옴
	if (UInputAction* InputAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		// 캐릭터에 해당 입력 액션과 함수를 바인딩
		BindAction(InputAction, TriggerEvent, ContextObject, Func);
	}
}

// 어빌리티 입력 액션 바인딩 구현
template<class UserObject, typename CallbackFunc>
inline void UPA_InputComponent::AbilityInputActionBind(const UDA_InputConfig* InInputConfig, UserObject* ContextObject, CallbackFunc InputPressedFunc, CallbackFunc InputReleasedFunc)
{
	check(InInputConfig);

	// 입력 구성 데이터 에셋의 어빌리티 입력 액션 배열을 순회
	for (const FInputActionConfig& AbilityInputAction : InInputConfig->AbilityInputActions)
	{
		// 해당 입력 액션 구성이 유효하지 않으면 패스
		if (!AbilityInputAction.IsValid()) continue;

		// 캐릭터에 해당 입력 액션의 시작(Started) 이벤트에 함수를 바인딩, 입력 태그를 지정
		BindAction(AbilityInputAction.InputAction, ETriggerEvent::Started, ContextObject, InputPressedFunc, AbilityInputAction.InputTag);
		
		// 캐릭터에 해당 입력 액션의 종료(Completed) 이벤트에 함수를 바인딩, 입력 태그를 지정
		BindAction(AbilityInputAction.InputAction, ETriggerEvent::Completed, ContextObject, InputReleasedFunc, AbilityInputAction.InputTag);
	}
}