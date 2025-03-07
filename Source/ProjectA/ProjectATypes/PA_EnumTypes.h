#pragma once

UENUM(BlueprintType)
enum class EPA_GameDifficulty : uint8
{
	Easy,
	Normal,
	Hard,
	VeryHard,
};

UENUM(BlueprintType)
enum class EPA_InputMode : uint8
{
	GameOnly,
	UIOnly,
};