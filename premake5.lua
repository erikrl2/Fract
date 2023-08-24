workspace "Fract"
    architecture "x64"
    startproject "Fract"
    configurations { "Debug", "Release" }
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	include "Fract"

	group "Dependencies"
		include "Fract/vendor/Glad"
		include "Fract/vendor/ImGui"
