workspace "ServerTest"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "ServerTest"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "ServerTest"