workspace "Steins"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Steins/vendor/GLFW/include"
IncludeDir["Glad"] = "Steins/vendor/Glad/include"
IncludeDir["ImGui"] = "Steins/vendor/imgui"

include "Steins/vendor/GLFW"
include "Steins/vendor/Glad"
include "Steins/vendor/imgui"

project "Steins"
	location "Steins"
	kind "SharedLib"
	language "C++"
	staticruntime "off"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "stspch.h"
	pchsource "Steins/src/stspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"STS_PLATFORM_WINDOWS",
			"STS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "STS_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "STS_RELEASE"
		runtime "Release"
		optimize "On"
	
	filter "configurations:Dist"
		defines "STS_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Steins/vendor/spdlog/include",
		"Steins/src"
	}

	links
	{
		"Steins"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"STS_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "STS_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "STS_RELEASE"
		runtime "Release"
		symbols "On"
	
	filter "configurations:Dist"
		defines "STS_DIST"
		runtime "Release"
		symbols "On"