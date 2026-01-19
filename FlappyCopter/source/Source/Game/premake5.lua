include "../../Premake/common.lua"

-------------------------------------------------------------
project "Game"
	location (dirs.projectfiles)
	dependson { "External", "Engine" }
		
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	debugdir "%{dirs.bin}"
	targetdir ("%{dirs.bin}")
	targetname("%{prj.name}_%{cfg.buildcfg}")
	objdir ("%{dirs.temp}/%{prj.name}/%{cfg.buildcfg}")

	links {"External", "Engine"}

	includedirs { 
		dirs.external, 
		dirs.engine,
		"../../CommonUtilities/include"
	}

	files {
		"source/**.h",
		"source/**.cpp",
	}

	libdirs { 
		dirs.lib, 
		dirs.dependencies,
		"../../CommonUtilities/lib"
	}
	
	verify_or_create_settings("Game")
	
	filter "configurations:Debug"
		defines {"_DEBUG"}
		runtime "Debug"
		symbols "on"
		files {"tools/**"}
		includedirs {"tools/"}
		links { "CommonUtilLib-d" }
		
	filter "configurations:Release"
		defines "_RELEASE"
		runtime "Release"
		optimize "on"
		files {"tools/**"}
		includedirs {"tools/"}
		links { "CommonUtilLib" }
		
	filter "configurations:Retail"
		defines "_RETAIL"
		runtime "Release"
		optimize "on"
		links { "CommonUtilLib" }

	filter "system:windows"
--		kind "StaticLib"
		staticruntime "off"
		symbols "On"		
		systemversion "latest"
		warnings "Extra"
		--conformanceMode "On"
		--buildoptions { "/permissive" }
		flags { 
		--	"FatalWarnings", -- would be both compile and lib, the original didn't set lib
			"FatalCompileWarnings",
			"MultiProcessorCompile"
		}
		
		defines {
			"WIN32",
			"_LIB", 
			"TGE_SYSTEM_WINDOWS" 
		}