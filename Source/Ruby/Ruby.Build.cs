// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ruby : ModuleRules
{
	public Ruby(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });


		// Add any macros that need to be set
		// PublicDefinitions.Add("WITH_MYTHIRDPARTYLIBRARY=1");

		// Add any include paths for the plugin
		PublicIncludePaths.Add("/Users/joe/mrubyc/src");
		PublicIncludePaths.Add("/Users/joe/mrubyc/hal/unreal");

		// Add any import libraries or static libraries
		PublicAdditionalLibraries.Add("/Users/joe/mrubyc/build/libmrubyc.a");
	}
}
