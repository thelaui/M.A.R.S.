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
      links { "sfml2-graphics", "sfml2-audio", "sfml2-system", "sfml2-window", "glu32", "opengl32" }
    elseif os.get() == "macosx" then
      links { "sfml2-graphics.framework", "sfml2-audio.framework", "sfml2-system.framework", "sfml2-window.framework", "opengl.framework" }
    else
      links { "sfml2-graphics", "sfml2-audio", "sfml2-system", "sfml2-window" }
      libdirs { "/usr/lib" }
    end
    
  configuration { "Debug*" }
    defines { "_DEBUG", "DEBUG" }
    flags   { "Symbols" }
    if os.get() == "windows" then
      links { "sfml2-graphics", "sfml2-audio", "sfml2-system", "sfml2-window", "glu32", "opengl32" }
    elseif os.get() == "macosx" then
      links { "sfml2-graphics.framework", "sfml2-audio.framework", "sfml2-system.framework", "sfml2-window.framework", "opengl.framework" }
    else
      links { "sfml2-graphics", "sfml2-audio", "sfml2-system", "sfml2-window" }
      libdirs { "/usr/lib" }
    end
