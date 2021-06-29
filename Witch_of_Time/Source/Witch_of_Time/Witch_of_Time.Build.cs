// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Witch_of_Time : ModuleRules
{
	public Witch_of_Time(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Niagara" });

	}
}
