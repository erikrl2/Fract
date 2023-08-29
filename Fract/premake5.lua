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
        "vendor/stb_image_write/include",
        "vendor/NativeFileDialog/include",
    }

    libdirs
    {
        "vendor/GLFW/lib",
    }

    links
    {
        "glfw3",
        "Glad",
        "ImGui",
        "stb_image_write",
        "NativeFileDialog",
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
