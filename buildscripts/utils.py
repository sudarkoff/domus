
import re
import socket
import time
import os
import os.path
import itertools
import subprocess
import sys

def getAllSourceFiles( arr=None , prefix="." ):
    if arr is None:
        arr = []

    for x in os.listdir( prefix ):
        if x.startswith( "." ) or x.startswith("build") or x.startswith( "buildscripts" ) or x.startswith( "third_party" ):
            continue
        full = prefix + "/" + x
        if os.path.isdir( full ) and not os.path.islink( full ):
            getAllSourceFiles( arr , full )
        else:
            if full.endswith( ".hpp" ) or full.endswith( ".cpp" ) or full.endswith( ".h" ) or full.endswith( ".c" ):
                arr.append( full )

    return arr


def getGitBranch():
    if not os.path.exists( ".git" ):
        return None

    version = open( ".git/HEAD" ,'r' ).read().strip()
    if not version.startswith( "ref: " ):
        return version
    version = version.split( "/" )
    version = version[len(version)-1]
    return version

def getGitBranchString( prefix="" , postfix="" ):
    t = re.compile( '[/\\\]' ).split( os.getcwd() )
    if len(t) > 2 and t[len(t)-1] == "domus":
        par = t[len(t)-2]
        m = re.compile( ".*_([vV]\d+\.\d+)$" ).match( par )
        if m is not None:
            return prefix + m.group(1).lower() + postfix
        if par.find("Nightly") > 0:
            return ""

    b = getGitBranch()
    if b == None or b == "master":
        return ""
    return prefix + b + postfix

def getGitVersion():
    if not os.path.exists( ".git" ):
        return "nogitversion"

    version = open( ".git/HEAD" ,'r' ).read().strip()
    if not version.startswith( "ref: " ):
        return version
    version = version[5:]
    f = ".git/" + version
    if not os.path.exists( f ):
        return version
    return open( f , 'r' ).read().strip()

def execsys( args ):
    import subprocess
    if isinstance( args , str ):
        r = re.compile( "\s+" )
        args = r.split( args )
    p = subprocess.Popen( args , stdout=subprocess.PIPE , stderr=subprocess.PIPE )
    r = p.communicate()
    return r;

def getprocesslist():
    raw = ""
    try:
        raw = execsys( "/bin/ps -ax" )[0]
    except Exception,e:
        print( "can't get processlist: " + str( e ) )

    r = re.compile( "[\r\n]+" )
    return r.split( raw )

def removeIfInList( lst , thing ):
    if thing in lst:
        lst.remove( thing )

def findVersion( root , choices ):
    for c in choices:
        if ( os.path.exists( root + c ) ):
            return root + c
    raise "can't find a version of [" + root + "] choices: " + choices

def choosePathExist( choices , default=None):
    for c in choices:
        if c != None and os.path.exists( c ):
            return c
    return default

def filterExists(paths):
    return filter(os.path.exists, paths)

def ensureDir( name ):
    d = os.path.dirname( name )
    if not os.path.exists( d ):
        print( "Creating dir: " + name );
        os.makedirs( d )
        if not os.path.exists( d ):
            raise "Failed to create dir: " + name

def distinctAsString( arr ):
    s = set()
    for x in arr:
        s.add( str(x) )
    return list(s)

def which(executable):
    if sys.platform == 'win32':
        paths = os.environ.get('Path', '').split(';')
    else:
        paths = os.environ.get('PATH', '').split(':')

    for path in paths:
        path = os.path.expandvars(path)
        path = os.path.expanduser(path)
        path = os.path.abspath(path)
        executable_path = os.path.join(path, executable)
        if os.path.exists(executable_path):
            return executable_path

    return executable

def find_python(min_version=(2, 5)):
    # if this script is being run by py2.5 or greater,
    # then we assume that "python" points to a 2.5 or
    # greater python VM. otherwise, explicitly use 2.5
    # which we assume to be installed.
    version = re.compile(r'[Pp]ython ([\d\.]+)', re.MULTILINE)
    binaries = ('python27', 'python2.7', 'python26', 'python2.6', 'python25', 'python2.5', 'python')
    for binary in binaries:
        try:
            out, err = subprocess.Popen([binary, '-V'], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
            for stream in (out, err):
                match = version.search(stream)
                if match:
                    versiontuple = tuple(map(int, match.group(1).split('.')))
                    if versiontuple >= (2, 5):
                        return which(binary)
        except:
            pass

    raise Exception('could not find suitable Python (version >= %s)' % '.'.join(min_version))
