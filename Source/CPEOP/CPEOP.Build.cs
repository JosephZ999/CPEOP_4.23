// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class CPEOP : ModuleRules
{
	public CPEOP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "SlateCore",
            }
        );

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "Settings",
            }
        );

        PublicIncludePaths.AddRange(new string[] {
            "CPEOP/public/Chars",
            "CPEOP/public/Chars/AI",
            "CPEOP/public/Chars/Components",
            "CPEOP/public/Chars/Heroes",
            "CPEOP/public/Chars/Monsters",

            "CPEOP/public/Objects",
            "CPEOP/public/Objects/Dynamic",
            "CPEOP/public/Objects/Static",

            "CPEOP/public/Sys",
            "CPEOP/public/Sys/Interfaces",
            "CPEOP/public/Sys/SaveGame"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
