using UnrealBuildTool;
using System.Collections.Generic;

public class GameJamPlus2024EditorTarget : TargetRules
{
	public GameJamPlus2024EditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("GameJamPlus2024");
	}
}