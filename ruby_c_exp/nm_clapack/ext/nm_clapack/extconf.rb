require 'mkmf'

def find_nmatrix
  nmatrix_config = dir_config('nmatrix',$sitearchdir,$sitearchdir)
  begin
    require 'rubygems'
    nm_gemspec=Gem::Specification.find_by_path('nmatrix.h')
    if nm_gemspec
      nmatrix_config = File.join(nm_gemspec.full_gem_path, nm_gemspec.require_path)
      $CPPFLAGS = " -I#{nmatrix_config} "+ $CPPFLAGS
    end
  rescue LoadError
  end

  have_nmatrix_h = have_header("nmatrix.h")

  if nmatrix_config
    if RUBY_PLATFORM =~ /cygwin|mingw/
      have_library("nmatrix")
    end
  end  
end

def find_headers
  find_nmatrix
  
  unless find_header('clapack.h')
    abort "CLAPACK is missing"
  end

  unless find_header('f2c.h')
    abort "CLAPACK is missing"
  end

  unless find_header('blaswrap.h')
    abort "CLAPACK is missing"
  end
end

def find_libraries
  unless find_library("tmglib",nil)
    abort "Cannot find tmglib_LINUX"
  end

  unless find_library("abclapack",nil)
    abort "Cannot find CLAPACK"
  end

  unless find_library("openblas",nil)
    abort "Cannot find openBLAS"
  end

  unless find_library("f2c",nil)
    abort "Cannot find F2C"
  end 
end

def set_linker_flags
  libs = ['-ltmglib', '-lopenblas', '-lf2c']
  libs.each do |lib|
    $LOCAL_LIBS << "#{lib} "
  end  
end

INCLUDE_DIRS  = [
  "/home/sameer/github_repos/scratch/ruby_c_exp/nm_clapack/ext/nm_clapack/include"
]
LIB_DIRS = [
  RbConfig::CONFIG["libdir"],
  # "/home/sameer/github_repos/scratch/ruby_c_exp/nm_clapack/ext/nm_clapack/lib"
]
dir_config("nm_clapack", INCLUDE_DIRS, LIB_DIRS)
# LIBPATHFLAG.prepend(" -L/home/sameer/github_repos/scratch/ruby_c_exp/nm_clapack/ext/nm_clapack/lib ")

find_headers
# find_libraries
# set_linker_flags
path = "/home/sameer/github_repos/scratch/ruby_c_exp/nm_clapack/ext/nm_clapack/lib"
CONFIG['LDSHAREDXX'] << " #{path}/libtmglib.a #{path}/libopenblas.a #{path}/libabclapack.a #{path}/libf2c.a "
$CPPFLAGS << " -g "

create_makefile('nm_clapack')

