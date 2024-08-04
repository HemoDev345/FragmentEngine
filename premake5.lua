workspace "FragmentEngine"
    architecture "x64"
    
    configurations {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "tracy"
    location "vender/tracy"
    kind "StaticLib"
    language "C++"
    cppdialect "C++11"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "vender/tracy/public/TracyClient.cpp",
    }

    links {
        "pthread",
        "dl"
    }

    defines {
        "TRACY_ENABLE",
    }

    filter "configurations:Debug"
        buildoptions "-g"
        defines "NDEBUG"
        runtime "Debug"
        symbols  "on"
    
    filter "configurations:Release"
        defines "DEBUG"
        runtime "Release"
        optimize  "on"
    
    filter "configurations:Dist"
        defines "DEBUG"
        runtime "Release"
        optimize  "on"


project "glad"
    location "vender/glad"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "vender/glad/src/glad.c",
    }
        
    includedirs {
        "vender/glad/include/"
    }

    links {
        "pthread",
        "dl"
    }

    defines {
        "TRACY_ENABLE",
    }

    filter "configurations:Debug"
        buildoptions "-g"
        defines "NDEBUG"
        runtime "Debug"
        symbols  "on"
    
    filter "configurations:Release"
        defines "DEBUG"
        runtime "Release"
        optimize  "on"
    
    filter "configurations:Dist"
        defines "DEBUG"
        runtime "Release"
        optimize  "on"
    
project "Fragment"
    location "Fragment" 
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
    }
    
    includedirs {
        "%{prj.name}/src",
        "vender/tracy/public",
        "vender/spdlog/include",
        "vender/glad/include",
    }

    filter "system:linux"        
        defines {
            "FM_PLATFORM_LINUX",
            "FM_RENDER_API_OPENGL"
        }

        links {
            'glad',
            'tracy',
            'X11',
            'GL'
        }

    filter "system:windows"
        
        defines {
            "FM_PLATFORM_WINDOWS",
        }

        links {
        }

    filter "configurations:Debug"
        defines "FM_DEBUG"
        buildoptions "-g"
        runtime "Debug"
        symbols  "on"
    
    filter "configurations:Release"
        defines "FM_RELEASE"
        runtime "Release"
        optimize  "on"
        
    filter "configurations:Dist"
        defines "FM_DIST"
        runtime "Release"
        optimize  "on"

project "Fragmentbox"
    location "Fragmentbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
    }
    
    includedirs {
        "%{prj.name}/src",
        "Fragment/src",
        "vender/tracy/public",
        "vender/spdlog/include",
        "vender/glad/include",
    }
    
    links {
        'Fragment'
    }

    filter "system:linux"        
        defines {
            "FM_PLATFORM_LINUX",
            "FM_RENDER_API_OPENGL"
        }

        links {
            'glad',
            'tracy',
            'X11',
            'GL'
        }

    filter "system:windows"
        
        defines {
            "FM_PLATFORM_WINDOWS",
        }
        
    filter "configurations:Debug"
        defines { "FM_DEBUG", "FM_PROFILE" }
        buildoptions "-g"
        runtime "Debug"
        symbols  "on"
    
    filter "configurations:Release"
        defines { "FM_RELEASE", "FM_PROFILE" }
        runtime "Release"
        optimize  "on"
        
    filter "configurations:Dist"
        defines "FM_DIST"
        runtime "Release"
        optimize  "on"  
