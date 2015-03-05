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

def set_linker_flags
  libs = ['-lopenblas']
  libs.each do |lib|
    $LOCAL_LIBS << "#{lib} "
  end  
end

def find_headers
  find_nmatrix

  unless find_header('lapacke.h')
    abort "Cannot find LAPACKE"
  end
end

def find_libs
  unless find_library('openblas',nil)
    abort "Cannot find openBLAS"
  end  
end

INCLUDE_DIRS  = [
  "/home/sameer/github_repos/scratch/ruby_c_exp/nm_lapacke/ext/nm_lapacke/include"
]
LIB_DIRS = [
  RbConfig::CONFIG["libdir"],
  "/home/sameer/github_repos/scratch/ruby_c_exp/nm_lapacke/ext/nm_lapacke/bin"
]
dir_config("nm_lapacke", INCLUDE_DIRS, LIB_DIRS)

find_libs
find_headers
set_linker_flags

create_makefile('nm_lapacke')