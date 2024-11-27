import platform
from datetime import date
from os import system
from os import environ

Files = []
Libs = []
Allowed = ["Linux"]
def IsAllowed():
    for a in Allowed: 
        # print("%s == %s -> %s"%(a,platform.system(),a==platform.system())) # debug only!
        if a==platform.system(): 
            return True
    return False

Today = date.today().strftime("%b %d %y")

print("\x1b[32mGyermo\x1b[33m\tBuilder script for the run-time environment\x1b[0m\n(c) Jeroen P. Broks\n")
def Doing(a,b): print("\x1b[93m%s: \x1b[96m%s\x1b[0m"%(a,b))
Doing("Platform",platform.system())
print("\n\n")
if not IsAllowed():
    print("\x1b[31mERROR!\x1b[0mThis script is NOT compatible with %s!"%platform.system())
    quit(1)

IncludeDirs = [
    "../../Libs/JCR6/Headers",
    "../../Libs/TQSL/Headers",
    "../../Libs/Units/Headers",
    "../../Libs/JCR6/3rdParty/zlib/src"
    "../../Libs/June19/head",
    "../../Libs/Kitty/Headers"
    ]


def Add(cat,files):
    for f in files: 
        Doing("Code "+cat,f)
        Files.append(f)

def AddLib(cat,files):
    for f in files: 
        Doing("Lib  "+cat,f)
        Libs.append(f)
        
# zlib        
if platform.system()=="Linux":
    Add("zlib",Glob("../../Libs/JCR6/3rdParty/zlib/src/*.c"))
else:
    AddLib("zlib",["../../Libs/JCR6/3rdParty/3rdParty/zlib/zlib"])
    
# project
#Environment(ENV = {'PATH' : environ['PATH']})
#Environment(ENV = {'PATH' : "/usr/local/lib"})
AddLib("SDL2",["SDL2","SDL2_mixer","SDL2_image","SDL2main"])

Add("JCR6",[    
    "../../Libs/JCR6/Source/JCR6_Core.cpp",
    "../../Libs/JCR6/Source/JCR6_JQL.cpp",
    "../../Libs/JCR6/Source/JCR6_RealDir.cpp",
    "../../Libs/JCR6/Source/JCR6_Write.cpp",
    "../../Libs/JCR6/Source/JCR6_zlib.cpp"])

Add("Kitty",Glob("../../Libs/Kitty/Source/*.cpp"))

Add("TQSL",[
    "../../Libs/TQSL/Source/TQSG.cpp",
    "../../Libs/TQSL/Source/TQSA.cpp",
    "../../Libs/TQSL/Source/TQSE.cpp"]
    )
Add("June 19", Glob("../../Libs/June19/src/*.cpp"))
Add("Units",[
    "../../Libs/Units/SlyvAsk.cpp",
    "../../Libs/Units/SlyvBank.cpp",
    "../../Libs/Units/SlyvDir.cpp",
    "../../Libs/Units/SlyvDirry.cpp",
    "../../Libs/Units/SlyvMD5.cpp",
    "../../Libs/Units/SlyvOpenURL.cpp",
    "../../Libs/Units/SlyvQCol.cpp",
    "../../Libs/Units/SlyvRequestFile.cpp",
    "../../Libs/Units/SlyvRoman.cpp",
    "../../Libs/Units/SlyvSharedArray.cpp",
    "../../Libs/Units/SlyvSTOI.cpp",
    "../../Libs/Units/SlyvStream.cpp",
    "../../Libs/Units/SlyvString.cpp",
    "../../Libs/Units/SlyvTime.cpp",
    "../../Libs/Units/SlyvVolumes.cpp"    
])
Add("Gyermo",Glob("Gyermo/*.cpp"))

Program("Exe/%s/Gyermo"%platform.system(),Files,CPPPATH=IncludeDirs)