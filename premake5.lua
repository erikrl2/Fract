workspace "Fract"
	architecture "x64"
	startproject "Fract"

	configurations { "Debug", "Release" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	project "Fract"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"

		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"src/**.h",
			"src/**.cpp",
			"vendor/Glad/src/glad.c"
		}

		includedirs
		{
			"src",
			"vendor/GLFW/include",
			"vendor/Glad/include"
		}

		libdirs
		{
			"vendor/GLFW/lib"
		}

		links
		{
			"glfw3"
		}

		filter "configurations:Debug"
			defines "DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "NDEBUG"
			optimize "On"
