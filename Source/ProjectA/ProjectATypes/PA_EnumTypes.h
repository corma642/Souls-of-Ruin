#pragma once

// ��ȿ ���� ������
UENUM()
enum class EPA_ValidType : uint8
{
	Valid,
	InValid,
};

// �Է� ��� ������
UENUM(BlueprintType)
enum class EPA_InputMode : uint8
{
	GameOnly,
	UIOnly,
};

// ���� ���� ������
UENUM(BlueprintType)
enum class EPA_SuccessType : uint8
{
	SuccessFul,
	Failed,
};