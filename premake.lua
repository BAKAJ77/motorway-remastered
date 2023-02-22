---@diagnostic disable: undefined-global

workspace "motorway"
    filename "motorway"
    configurations { "debug", "release" }
    architecture "x86_64"

------------------------------------------------------------------------------------------------------------------------------------------------

project "motorway"
    filename "motorway"
    staticruntime "on"
    language "C++"
    cppdialect "C++17"

    targetname "motorway"
    targetdir "bin/%{cfg.buildcfg}/"
    objdir "objs/%{prj.name}/%{cfg.buildcfg}/"

    includedirs { "src", "libs/irrklang/include", "libs/glad/include", "libs/glfw/include", "libs/json/include", "libs/glm", "libs/stb", }

    files { "src/**.h", "src/**.cpp", "src/**.c", "src/**.tpp" }

    -- Project platform define macro based on identified system
    filter "system:windows"
        defines "_PLATFORM_WINDOWS"

    filter "system:macosx"
        defines "_PLATFORM_MACOSX"

    -- Project settings with values unique to the Debug/Release configurations
    filter "configurations:debug"
        kind "ConsoleApp"
        
        libdirs { "libs/irrklang/bin/win64", "libs/glfw/build/src/Debug" }
        links { "glfw3", "irrKlang" }

        defines { "_DEBUG" }
        symbols "On"

    filter "configurations:release"
        kind "WindowedApp"
        entrypoint "mainCRTStartup"

        libdirs { "libs/irrklang/bin/win64", "libs/glfw/build/src/Release" }
        links { "glfw3", "irrKlang" }

        defines { "NDEBUG" }
        optimize "Speed"

    -- Copy required DLL lib files into game executable directory when building
    filter { "system:windows", "configurations:debug" }
        postbuildcommands { "copy libs\\irrklang\\bin\\win64\\irrKlang.dll bin\\debug\\irrKlang.dll",
            "copy libs\\irrklang\\bin\\win64\\ikpMP3.dll bin\\debug\\ikpMP3.dll" }

    filter { "system:windows", "configurations:release" }
        postbuildcommands { "copy libs\\irrklang\\bin\\win64\\irrKlang.dll bin\\release\\irrKlang.dll",
            "copy libs\\irrklang\\bin\\win64\\ikpMP3.dll bin\\release\\ikpMP3.dll" }

------------------------------------------------------------------------------------------------------------------------------------------------
