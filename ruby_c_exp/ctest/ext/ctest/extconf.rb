require 'mkmf'
require 'awesome_print'

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
# CONFIG['CXX'] = "g++"
# ap CONFIG
have_library('stdc++')
create_makefile('ctest')