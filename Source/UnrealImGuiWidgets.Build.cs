using System.IO;
using UnrealBuildTool;

namespace UnrealBuildTool.Rules
{
	public class UnrealImGuiWidgets : ModuleRules
	{
		public UnrealImGuiWidgets(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

			PublicIncludePaths.AddRange(new string[]
			{
				ModuleDirectory + "/Public",
			});
 
			PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"InputCore",
				"Slate",
				"AngelscriptCode",
				"ImGui",
				"DeveloperSettings",
				"AngelscriptImGui"
			});
		}
	}
}
