require('vstudio')

local vs = premake.vstudio.vc2010

local function premakeVersionComment(prj)
    premake.w('<!-- Generated by Premake ' .. _PREMAKE_VERSION .. ' -->')
end

local function vcpkg(prj)
    premake.w('<VcpkgTriplet>x64-windows-static</VcpkgTriplet>')
    premake.w('<VcpkgEnabled>false</VcpkgEnabled>')
    premake.w('<VcpkgManifestInstall>false</VcpkgManifestInstall>')
    premake.w('<VcpkgAutoLink>false</VcpkgAutoLink>')  
end

premake.override(premake.vstudio.vc2010.elements, "project", function(base, prj)
    local calls = base(prj)
    table.insertafter(calls, vs.xmlDeclaration, premakeVersionComment)
    return calls
end)

premake.override(premake.vstudio.vc2010.elements, "globals", function(base, prj)
    local calls = base(prj)
    table.insertafter(calls, vs.globals, vcpkg)
    return calls
end)

workspace "Steins"
	architecture "x64"
	startproject "Gate"

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
IncludeDir["stb_image"] = "Steins/vendor/stb_image"

group "Dependencies"
	include "Steins/vendor/GLFW"
	include "Steins/vendor/Glad"
	include "Steins/vendor/imgui"
group ""

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
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"d3d11.lib"
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


project "SteinsGate"
	location "SteinsGate"
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