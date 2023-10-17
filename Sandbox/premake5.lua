project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Steins/vendor/spdlog/include",
		"%{wks.location}/Steins/src",
		"%{wks.location}/Steins/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Steins"
	}

	filter "system:windows"
		systemversion "latest"

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