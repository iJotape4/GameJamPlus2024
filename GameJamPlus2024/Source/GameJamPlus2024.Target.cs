using UnrealBuildTool;
using System.Collections.Generic;

public class GameJamPlus2024Target : TargetRules
{
	public GameJamPlus2024Target(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;  // Asegúrate de que sea TargetType.Game
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("GameJamPlus2024");
	}
}