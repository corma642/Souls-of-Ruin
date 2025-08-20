#pragma once

// 확인 여부 열거형
UENUM()
enum class EPA_ConfirmType : uint8
{
	Yes,
	No,
};

// 유효 여부 열거형
UENUM()
enum class EPA_ValidType : uint8
{
	Valid,
	InValid,
};

// 입력 모드 열거형
UENUM()
enum class EPA_InputMode : uint8
{
	GameOnly,
	UIOnly,
};

// 성공 여부 열거형
UENUM()
enum class EPA_SuccessType : uint8
{
	SuccessFul,
	Failed,
};
