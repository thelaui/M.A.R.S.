solution "MarsProject"
  configurations { "Release", "Debug" }

project "mars"
  language "C++"
  kind     "ConsoleApp"
  files  { "include/**.hpp", "src/**.cpp", "data/shaders/*", "resources/*"  }
  includedirs { "include/" }
 
  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" }
    if os.get() == "windows" then
      links { "sfml-graphics", "sfml-audio", "sfml-system", "sfml-window", "glu32", "opengl32", "fribidi-0", "tag" }
    elseif os.get() == "macosx" then
      links { "sfml-graphics.framework", "sfml-audio.framework", "sfml-system.framework", "sfml-window.framework", "opengl.framework", "fribidi", "tag" }
    else
      links { "sfml-graphics", "sfml-audio", "sfml-system", "sfml-window", "Xrandr", "fribidi", "tag" }
      libdirs { "/usr/lib", "/usr/local/lib" }
    end
    
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }
    if os.get() == "windows" then
      links { "sfml-graphics", "sfml-audio", "sfml-system", "sfml-window", "glu32", "opengl32", "fribidi-0", "tag" }
    elseif os.get() == "macosx" then
      links { "sfml-graphics.framework", "sfml-audio.framework", "sfml-system.framework", "sfml-window.framework", "opengl.framework", "fribidi", "tag" }
    else
      links { "sfml-graphics", "sfml-audio", "sfml-system", "sfml-window", "Xrandr", "fribidi", "tag" }
      libdirs { "/usr/lib", "/usr/local/lib" }
    end
