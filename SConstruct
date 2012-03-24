# -*- mode: python; -*-
# build file for Domus
# requires scons: http://www.scons.org
# type 'scons' to build

# This file, SConstruct, configures the build environment, and then delegates
# to several, subordinate SConscript files, which describe specific build
# rules.

EnsureSConsVersion( 1, 1, 0 )

import os
import sys
import types
from buildscripts import utils

import libdeps

# --- options ----

options = {}

def add_option( name, help , nargs , contibutesToVariantDir , dest=None ):

    if dest is None:
        dest = name

    AddOption( "--" + name , 
               dest=dest,
               type="string",
               nargs=nargs,
               action="store",
               help=help )

    options[name] = { "help" : help ,
                      "nargs" : nargs , 
                      "contibutesToVariantDir" : contibutesToVariantDir ,
                      "dest" : dest } 

def get_option( name ):
    return GetOption( name )

def has_option( name ):
    x = get_option( name )
    if x is None:
        return False

    if x == False:
        return False

    if x == "":
        return False

    return True


def get_variant_dir():
    
    a = []
    
    for name in options:
        o = options[name]
        if not has_option( o["dest"] ):
            continue
        if not o["contibutesToVariantDir"]:
            continue
        
        if o["nargs"] == 0:
            a.append( name )
        else:
            x = get_option( name )
            x = re.sub( "[,\\\\/]" , "_" , x )
            a.append( name + "_" + x )
            
    s = "#build/${PYSYSPLATFORM}/"

    if len(a) > 0:
        a.sort()
        s += "/".join( a ) + "/"
    else:
        s += "normal/"
    return s
        
# linking options
add_option( "release" , "release build" , 0 , True )
add_option( "static" , "fully static build" , 0 , True )

# base compile flags
add_option( "64" , "force 64 bit" , 0 , True , "force64" )
add_option( "32" , "force 32 bit" , 0 , True , "force32" )

add_option( "cpppath", "Include path if you have headers in a nonstandard directory" , 1 , True )
add_option( "libpath", "Library path if you have libraries in a nonstandard directory" , 1 , True )

# dev options
add_option( "d", "debug build no optimization, etc..." , 0 , True , "debugBuild" )

# don't run configure if user calls --help
if GetOption('help'):
    Return()

# --- environment setup ---

variantDir = get_variant_dir()

def print_local_info():
    import sys, SCons
    print( "scons version: " + SCons.__version__ )
    print( "python version: " + " ".join( [ `i` for i in sys.version_info ] ) )

print_local_info()

boostLibs = [ "program_options" ]

nix = False
linux = False
linux64  = False
darwin = False
force32 = has_option( "force32" ) 
force64 = has_option( "force64" )
if not force64 and not force32 and os.getcwd().endswith( "domus-64" ):
    force64 = True
    print( "*** assuming you want a 64-bit build b/c of directory *** " )

release = has_option( "release" )
static = has_option( "static" )

debugBuild = has_option( "debugBuild" )

env = Environment( BUILD_DIR=variantDir,
                   tools=["default"],
                   PYSYSPLATFORM=os.sys.platform,
                   )


libdeps.setup_environment( env )

extraLibPlaces = []

env['EXTRACPPPATH'] = []
env['EXTRALIBPATH'] = []

def add_extra_libs( s ):
    for x in s.split(","):
        env.Append( EXTRACPPPATH=[ x + "/include" ] )
        env.Append( EXTRALIBPATH=[ x + "/lib" ] )
        env.Append( EXTRALIBPATH=[ x + "/lib64" ] )
        extraLibPlaces.append( x + "/lib" )

# ---- other build setup -----

platform = os.sys.platform
if "uname" in dir(os):
    processor = os.uname()[4]
else:
    processor = "i386"

if force32:
    processor = "i386"
if force64:
    processor = "x86_64"

env['PROCESSOR_ARCHITECTURE'] = processor

def filter_exists(paths):
    return filter(os.path.exists, paths)

if "darwin" == os.sys.platform:
    darwin = True
    platform = "osx" # prettier than darwin

    if env["CXX"] is None:
        print( "YO" )
        if os.path.exists( "/usr/bin/g++-4.2" ):
            env["CXX"] = "g++-4.2"

    nix = True

    if force64:
       env.Append( EXTRACPPPATH=["/usr/64/include"] )
       env.Append( EXTRALIBPATH=["/usr/64/lib"] )
       if installDir == DEFAULT_INSTALL_DIR and not distBuild:
           installDir = "/usr/64/"
    else:
       env.Append( EXTRACPPPATH=filter_exists(["/sw/include" , "/opt/local/include"]) )
       env.Append( EXTRALIBPATH=filter_exists(["/sw/lib/", "/opt/local/lib"]) )

elif os.sys.platform.startswith("linux"):
    linux = True
    platform = "linux"

    env.Append( LIBS=['m'] )

    if os.uname()[4] == "x86_64" and not force32:
        linux64 = True
        nixLibPrefix = "lib64"
        env.Append( EXTRALIBPATH=["/usr/lib64" , "/lib64" ] )
        env.Append( LIBS=["pthread"] )

        force64 = False

    if force32:
        env.Append( EXTRALIBPATH=["/usr/lib32"] )

    nix = True

    if static:
        env.Append( LINKFLAGS=" -static " )

else:
    print( "No special config for [" + os.sys.platform + "] which probably means it won't work" )

env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME'] = 1
if nix:
    # -Winvalid-pch Warn if a precompiled header (see Precompiled Headers) is found in the search path but can't be used. 
    env.Append( CPPFLAGS="-fPIC -fno-strict-aliasing -ggdb -pthread -Wall -Wsign-compare -Wno-unknown-pragmas -Winvalid-pch" )

    if linux:
        env.Append( CPPFLAGS=" -Werror -pipe " )
        if not has_option('clang'):
            env.Append( CPPFLAGS=" -fno-builtin-memcmp " ) # glibc's memcmp is faster than gcc's

    env.Append( CPPDEFINES="_FILE_OFFSET_BITS=64" )
    env.Append( CXXFLAGS=" -Wnon-virtual-dtor -Woverloaded-virtual" )
    env.Append( LINKFLAGS=" -fPIC -pthread -rdynamic" )
    env.Append( LIBS=[] )

    #make scons colorgcc friendly
    env['ENV']['HOME'] = os.environ['HOME']
    try:
        env['ENV']['TERM'] = os.environ['TERM']
    except KeyError:
        pass

    if debugBuild:
        env.Append( CPPFLAGS=" -O0 -fstack-protector " );
        env['ENV']['GLIBCXX_FORCE_NEW'] = 1; # play nice with valgrind
    else:
        env.Append( CPPFLAGS=" -O3 " )
        #env.Append( CPPFLAGS=" -fprofile-generate" )
        #env.Append( LINKFLAGS=" -fprofile-generate" )
        # then:
        #env.Append( CPPFLAGS=" -fprofile-use" )
        #env.Append( LINKFLAGS=" -fprofile-use" )

    if force64:
        env.Append( CFLAGS="-m64" )
        env.Append( CXXFLAGS="-m64" )
        env.Append( LINKFLAGS="-m64" )

    if force32:
        env.Append( CFLAGS="-m32" )
        env.Append( CXXFLAGS="-m32" )
        env.Append( LINKFLAGS="-m32" )

try:
    umask = os.umask(022)
except OSError:
    pass

commonFiles = []
serverOnlyFiles = []

env['DOMUS_COMMON_FILES'] = commonFiles
env['DOMUS_SERVER_ONLY_FILES' ] = serverOnlyFiles

env.Append( CPPPATH=['$EXTRACPPPATH'],
            LIBPATH=['$EXTRALIBPATH'] )

# --- check system ---

def getSysInfo():
    return " ".join( os.uname() )

def do_configure( myenv , shell=False ):
    conf = Configure(myenv)
    myenv["LINKFLAGS_CLEAN"] = list( myenv["LINKFLAGS"] )
    myenv["LIBS_CLEAN"] = list( myenv["LIBS"] )

    if 'CheckCXX' in dir( conf ):
        if  not conf.CheckCXX():
            print( "c++ compiler not installed!" )
            Exit(1)

    if nix and not shell:
        if not conf.CheckLib( "stdc++" ):
            print( "can't find stdc++ library which is needed" );
            Exit(1)

    def my_check_lib( poss , failIfNotFound=False , staticOnly=False):

        if type( poss ) != types.ListType :
            poss = [poss]

        allPlaces = [];
        allPlaces += extraLibPlaces
        if nix and release:
            allPlaces += myenv.subst( myenv["LIBPATH"] )
            if not force64:
                allPlaces += [ "/usr/lib" , "/usr/local/lib" ]

            for p in poss:
                for loc in allPlaces:
                    fullPath = loc + "/lib" + p + ".a"
                    if os.path.exists( fullPath ):
                        myenv.Append( _LIBFLAGS='${SLIBS}',
                                      SLIBS=" " + fullPath + " " )
                        return True

        if release and failIfNotFound:
            print( "ERROR: can't find static version of: " + str( poss ) + " in: " + str( allPlaces ) )
            Exit(1)

        res = not staticOnly and conf.CheckLib( poss )
        if res:
            return True

        if failIfNotFound:
            print( "can't find or link against library " + str( poss ) + " in " + str( myenv["LIBPATH"] ) )
            print( "see config.log for more information" )
            Exit(1)

        return False

    # check boost lib
    if not conf.CheckCXXHeader( "boost/filesystem/operations.hpp" ):
        print( "can't find boost headers" )
        Exit(1)

    # this will add it if it exists and works
    my_check_lib( [ "boost_system-mt", "boost_system" ] )

    for b in boostLibs:
        l = "boost_" + b
        my_check_lib( [ l + "-mt" , l ] , release or not shell)

    if not conf.CheckCXXHeader( "execinfo.h" ):
        myenv.Append( CPPDEFINES=[ "NOEXECINFO" ] )

    return conf.Finish()

env = do_configure( env )

#  ---- Docs ----
def build_docs(env, target, source):
    """Generate documentation.
    """
    from buildscripts import docs
    docs.main()

env.Alias("docs", [], [build_docs])
env.AlwaysBuild("docs")

#  ---- astyle ----
def astyle( env , target , source ):
    """Re-formatt the source code
    """
    res = utils.execsys( "astyle --version" )
    res = " ".join(res)
    if res.count( "2." ) == 0:
        print( "astyle 2.x needed, found:" + res )
        Exit(-1)

    files = utils.getAllSourceFiles() 
    files = filter( lambda x: not x.endswith( ".c" ) , files )

    cmd = "astyle --options=domus_astyle " + " ".join( files )
    res = utils.execsys( cmd )
    print( res[0] )
    print( res[1] )

env.Alias( "style" , [] , [ astyle ] )
env.AlwaysBuild( "style" )

#  ---- CONVENIENCE ----

def tabs( env, target, source ):
    """Check for TABS in C++ source files
    """
    from subprocess import Popen, PIPE
    from re import search, match
    diff = Popen( [ "git", "diff", "-U0", "origin/master", "master" ], stdout=PIPE ).communicate()[ 0 ]
    sourceFile = False
    for line in diff.split( "\n" ):
        if match( "diff --git", line ):
            sourceFile = not not search( "\.(h|hpp|c|cpp)\s*$", line )
        if sourceFile and match( "\+ *\t", line ):
            return True # TODO: Return a proper error message ("TAB in %s" % line)
    return False

env.Alias( "checkSource", [], [ tabs ] )
env.AlwaysBuild( "checkSource" )

def git_push( env, target, source ):
    """Push to the tracking remote branch.
    """
    import subprocess
    return subprocess.call( [ "git", "push" ] )

# TODO: Add running smoke tests here
env.Alias( "push", [ ".", "checkSource" ], git_push )
env.AlwaysBuild( "push" )

# The following symbols are exported for use in subordinate SConscript files.
# Ideally, the SConscript files would be purely declarative.  They would only
# import build environment objects, and would contain few or no conditional
# statements or branches.
#
# Currently, however, the SConscript files do need some predicates for
# conditional decision making that hasn't been moved up to this SConstruct file,
# and they are exported here, as well.
Export("env")
Export("has_option")
Export("getSysInfo")
Export("darwin linux nix")

env.SConscript( 'src/SConscript', variant_dir=variantDir, duplicate=False )
