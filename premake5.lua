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
IncludeDir["glm"] = "Steins/vendor/glm"

include "Steins/vendor/GLFW"
include "Steins/vendor/Glad"
include "Steins/vendor/imgui"

project "Steins"
	location "Steins"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"


	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "stspch.h"
	pchsource "Steins/src/stspch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"STS_PLATFORM_WINDOWS",
			"STS_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "STS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "STS_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "STS_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Steins/src",
		"Steins/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Steins"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"STS_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "STS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "STS_RELEASE"
		runtime "Release"
		symbols "on"
	
	filter "configurations:Dist"
		defines "STS_DIST"
		runtime "Release"
		symbols "on"