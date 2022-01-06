workspace "HazelEngine"
    architecture "x64"

    configurations{
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "HazelEngine"
    location "HazelEngine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-init/"..outputdir.."/%{prj.name}")

    files{
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs{
        "%{prj.name}/HazelEngine/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        --宏
        defines{
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL"
        }

        --将dll从HazelEngine移到Sandbox中
        postbuildcommands{
            ("{COPY} %(cfg.buildtarget.relpath) ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configuration:Debug"
        defines "HZ_DEBUG"
        symbols "On"

    filter "configuration:Release"
        defines "HZ_RELEASE"
        symbols "On"

    filter "configuration:Dist"
        defines "HZ_DIST"
        symbols "On"
    
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    
    targetdir ("bin/"..outputdir.."/%{prj.name}")
    objdir ("bin-init/"..outputdir.."/%{prj.name}")

    files{
        "%{prj.name}/Sandbox/src/**.h",
        "%{prj.name}/Sandbox/src/**.cpp",
    }

    includedirs{
        "HazelEngine/HazelEngine/vendor/spdlog/include",
        "HazelEngine/src"
    }

    links{
        "HazelEngine"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        --宏
        defines{
            "HZ_PLATFORM_WINDOWS",
        }
    
    filter "configuration:Debug"
        defines "HZ_DEBUG"
        symbols "On"

    filter "configuration:Release"
        defines "HZ_RELEASE"
        symbols "On"

    filter "configuration:Dist"
        defines "HZ_DIST"
        symbols "On"