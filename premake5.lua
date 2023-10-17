include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Steins"
	architecture "x64"
	startproject "SteinsGate"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Steins/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Steins/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Steins/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Steins/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Steins/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Steins/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/Steins/vendor/yaml_cpp/include"

group "Dependencies"
	include "vendor/premake"
	include "Steins/vendor/GLFW"
	include "Steins/vendor/Glad"
	include "Steins/vendor/imgui"
	include "Steins/vendor/yaml-cpp"
group ""

include "Steins"
include "Sandbox"
include "SteinsGate"