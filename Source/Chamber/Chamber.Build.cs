// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using UnrealBuildTool.Rules;

public class Chamber : ModuleRules
{
	public Chamber(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "AIModule", "Niagara", "CoreUObject", "Engine", "InputCore", "UMG", "EnhancedInput" });

        

    }
}
