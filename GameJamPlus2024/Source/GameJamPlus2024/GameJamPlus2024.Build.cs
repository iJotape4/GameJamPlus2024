// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameJamPlus2024 : ModuleRules
{
	public GameJamPlus2024(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
