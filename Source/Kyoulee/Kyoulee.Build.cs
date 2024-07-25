using UnrealBuildTool;

public class Kyoulee : ModuleRules
{
    public Kyoulee(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" , "UMG"});
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    }
}