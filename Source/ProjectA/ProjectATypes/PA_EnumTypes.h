#pragma once

// 유효 여부 열거형
UENUM()
enum class EPA_ValidType : uint8
{
	Valid,
	InValid,
};

// 입력 모드 열거형
UENUM(BlueprintType)
enum class EPA_InputMode : uint8
{
	GameOnly,
	UIOnly,
};

// 성공 여부 열거형
UENUM(BlueprintType)
enum class EPA_SuccessType : uint8
{
	SuccessFul,
	Failed,
};