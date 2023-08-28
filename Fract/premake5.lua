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
    }

    includedirs
    {
        "src",
        "vendor/GLFW/include",
        "vendor/Glad/include",
        "vendor/ImGui/include",
        "vendor/stb/include",
        "vendor/nfd/include",
    }

    libdirs
    {
        "vendor/GLFW/lib",
    }

    links
    {
        "Glad",
        "glfw3",
        "ImGui",
        "stb",
        "nfd",
    }

    defines
    {
        "GLFW_INCLUDE_NONE",
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"
