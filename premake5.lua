workspace "Steins"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Steins"
	location "Steins"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"STS_PLATFORM_WINDOWS",
			"STS_BUILD_DLL",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "STS_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "STS_RELEASE"
		symbols "On"
	
	filter "configurations:Dist"
		defines "STS_DIST"
		symbols "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		symbols "On"

	filter "configurations:Release"
		defines "STS_RELEASE"
		symbols "On"
	
	filter "configurations:Dist"
		defines "STS_DIST"
		symbols "On"