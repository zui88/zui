workspace "zui"
    architecture "x86_64"
    language "c++"
    cppdialect "c++17"

    configurations 
    {
       "Debug",
       "Release"
    }


-- variable for right objpath
obj_path = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "zui"
    -- place the makefile into its own directory
    -- location "zui"
    kind "StaticLib"
    staticruntime "on"
    targetdir ("bin/"  .. obj_path .. "/%{prj.name}")
    objdir ("bin-int/" .. obj_path .. "/%{prj.name}")

    files
    {
       "%{prj.name}/src/zui/**.h",
       "%{prj.name}/src/zui/**.cpp",
    }
    -- removefiles "%{prj.name}/**/test/**"

    filter "configurations:Debug"
	defines "PX_DEBUG"
	symbols "On"

    filter "configurations:Release"
	defines "PX_RELEASE"
	optimize "On"
