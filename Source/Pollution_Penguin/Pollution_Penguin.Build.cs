// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Pollution_Penguin : ModuleRules
{
	public Pollution_Penguin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Niagara", "GameplayTasks", "AIModule"});
	
		PrivateIncludePaths.Add("Pollution_Penguin");
		PrivateIncludePaths.Add("Pollution_Penguin/*");
		PrivateIncludePaths.Add("Pollution_Penguin/AI");
		PrivateIncludePaths.Add("Pollution_Penguin/Enemy");
		PrivateIncludePaths.Add("Pollution_Penguin/Player");
		PrivateIncludePaths.Add("Pollution_Penguin/Map");
		
	}
}
