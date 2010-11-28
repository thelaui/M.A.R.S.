solution "MarsProject"
  configurations { "Release", "Debug" }

project "Mars"
  language "C++"
  kind     "ConsoleApp"
  files  { "**.hpp", "**.cpp", "data/shaders/*" }
  includedirs { "include/" }
 
  configuration { "Release*" }
    defines { "NDEBUG" }
    flags   { "Optimize" }
    if os.get() == "windows" then
      links { "sfml-graphics", "sfml-audio", "sfml-system", "sfml-window", "glu32", "opengl32" }
    elseif os.get() == "macosx" then
      links { "sfml-graphics.framework", "sfml-audio.framework", "sfml-system.framework", "sfml-window.framework", "opengl.framework" }
    else
      links { "sfml-graphics", "sfml-audio", "sfml-system", "sfml-window" }
      libdirs { "/usr/lib" }
    end
    
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }
    if os.get() == "windows" then
      links { "sfml-graphics", "sfml-audio", "sfml-system", "sfml-window", "glu32", "opengl32" }
    elseif os.get() == "macosx" then
      links { "sfml-graphics.framework", "sfml-audio.framework", "sfml-system.framework", "sfml-window.framework", "opengl.framework" }
    else
      links { "sfml-graphics", "sfml-audio", "sfml-system", "sfml-window" }
      libdirs { "/usr/lib" }
    end
