project "NativeFileDialog"
    kind "StaticLib"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files { "src/nfd_common.c" }
    includedirs { ".", "include" }

    filter "system:windows"
        language "C++"
        files { "src/nfd_win.cpp" }
        defines { "UNICODE", "_UNICODE" }

    filter {"system:linux"}
        language "C"
        files { "src/nfd_gtk.c" }
        buildoptions { "`pkg-config --cflags gtk+-3.0`" }

    filter "system:macosx"
        language "C"
        files { "src/nfd_cocoa.m" }

    filter "action:vs*"
        defines { "_CRT_SECURE_NO_WARNINGS" }

    filter {"action:gmake or action:xcode4"}
        buildoptions { "-fno-exceptions" }

    filter "configurations:Debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
        optimize "On"
