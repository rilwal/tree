workspace "Tree"
   configurations { "Debug", "Release" }
   architecture "x86_64"

   flags
	{
		"MultiProcessorCompile"
	}
    
project "GLFW"
   kind "StaticLib"

   defines
   {
      "_GLFW_WIN32"
   }

   files
   {
      "vendor/glfw/src/**.c",
      "vendor/glfw/src/**.h"
   }



project "Engine"
    kind "ConsoleApp"
    language "c++"
    cppdialect "C++latest"

    files 
    {
      "src/**.cpp",
      "src/**.hpp",
      "vendor/glad/src/gl.c",
      "vendor/imgui/*.cpp",
      "vendor/imgui/backends/imgui_impl_opengl3.cpp",
      "vendor/imgui/backends/imgui_impl_glfw.cpp"

    }

    includedirs 
    {
      "src",
      "vendor/glad/include",
      "vendor/glm/glm",
      "vendor/glfw/include",
      "vendor/imgui",
    }

    links {
      "GLFW"
    }