// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameJamPlus2024 : ModuleRules
{
	public GameJamPlus2024(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Public dependencies used in both runtime and editor builds
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"Paper2D",
			"UMG"  // Use UMG only, not UMGEditor
		});

		// Private dependencies for internal use
		PrivateDependencyModuleNames.AddRange(new string[] {
			"AITestSuite"
		});

		// Include UMGEditor only in editor builds
		if (Target.bBuildEditor)
		{
			PrivateDependencyModuleNames.Add("UMGEditor");
		}
	}
}