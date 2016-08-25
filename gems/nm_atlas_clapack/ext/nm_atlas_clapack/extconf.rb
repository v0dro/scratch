require "mkmf"

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

def find_atlas_clapack
  if have_header("atlas/cblas.h")
    have_header("atlas/clapack.h")
  else
    have_header("cblas.h")
    have_header("clapack.h")
  end

  have_func("clapack_dgetrf", "atlas/clapack.h")
  have_func("clapack_dgetri", "atlas/clapack.h")
  have_func("dgesvd_", "clapack.h") # This may not do anything. dgesvd_ seems to be in LAPACK, not CLAPACK.
  have_func("cblas_dgemm", "cblas.h")
end

def set_linker_flags
  $libs += " -llapack -lcblas -latlas "
end

def find_headers
  find_nmatrix
  find_atlas_clapack
end

def find_libs
  idefaults = {lapack: ["/usr/include/atlas"],
               cblas: ["/usr/local/atlas/include", "/usr/include/atlas"],
               atlas: ["/usr/local/atlas/include", "/usr/include/atlas"]}

  ldefaults = {lapack: ["/usr/local/lib", "/usr/local/atlas/lib", "/usr/lib64/atlas"].delete_if { |d| !Dir.exists?(d) },
               cblas: ["/usr/local/lib", "/usr/local/atlas/lib", "/usr/lib64/atlas"].delete_if { |d| !Dir.exists?(d) },
               atlas: ["/usr/local/lib", "/usr/local/atlas/lib", "/usr/lib", "/usr/lib64/atlas"].delete_if { |d| !Dir.exists?(d) }}

  unless have_library("lapack")
    dir_config("lapack", idefaults[:lapack], ldefaults[:lapack])
  end

  unless have_library("cblas")
    dir_config("cblas", idefaults[:cblas], ldefaults[:cblas])
  end

  unless have_library("atlas")
    dir_config("atlas", idefaults[:atlas], ldefaults[:atlas])
  end
end

INCLUDE_DIRS  = [
  RbConfig::CONFIG['includedir']#,
  # "/home/sameer/github_repos/scratch/ruby_c_exp/nm_atlas/ext/nm_atlas/include"
]
LIB_DIRS = [
  RbConfig::CONFIG["libdir"]
]
dir_config("nm_atlas", INCLUDE_DIRS, LIB_DIRS)

find_libs
find_headers
set_linker_flags

create_makefile('nm_atlas_clapack')

