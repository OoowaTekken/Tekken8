using UnrealBuildTool;

public class JMH : ModuleRules
{
    public JMH(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" , "UMG"});
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

    }
}