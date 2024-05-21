// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UE_AssignmentUI : ModuleRules
{
	public UE_AssignmentUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","EnhancedInput","UMG", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "Assignment_1_2" });



        PrivateDependencyModuleNames.AddRange(new string[] {  });

    /*    PublicIncludePaths.AddRange(new string[] {
          "D:/UE/Assignment4/Plugins/Assignment_1_2/Source/Assignment_1_2/Public" });*/

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
