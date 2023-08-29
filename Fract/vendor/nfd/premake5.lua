  project "nfd"
    kind "StaticLib"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/*.h",
        "src/nfd_common.c",
        "include/*.h",
    }

    includedirs
    {
        ".",
    }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"

    filter "system:windows"
        language "C++"
        files { "src/nfd_win.cpp" }
        defines {"UNICODE", "_UNICODE"}

    filter {"action:gmake or action:xcode4"}
        buildoptions { "-fno-exceptions" }

    filter "system:macosx"
        language "C"
        files { "src/nfd_cocoa.m" }

    filter {"system:linux", "options:linux_backend=gtk3"}
        language "C"
        files { "src/nfd_gtk.c" }
        buildoptions { "`pkg-config --cflags gtk+-3.0`" }

    filter {"system:linux", "options:linux_backend=zenity"}
        language "C"
        files { "src/nfd_zenity.c" }

    filter "action:vs*"
        defines { "_CRT_SECURE_NO_WARNINGS" }
