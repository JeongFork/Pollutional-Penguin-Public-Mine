// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Pollution_PenguinEditorTarget : TargetRules
{
	public Pollution_PenguinEditorTarget(TargetInfo Target) : base(Target)
	{
        bOverrideBuildEnvironment = true;
        Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
	}
}
