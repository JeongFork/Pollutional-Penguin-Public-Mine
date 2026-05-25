// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Pollution_PenguinTarget : TargetRules
{
	public Pollution_PenguinTarget(TargetInfo Target) : base(Target)
	{
        bOverrideBuildEnvironment = true;
        Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
	}
}
