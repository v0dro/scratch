require 'nmatrix'
require 'nm_interface.rb'

if File.exist?("lib/ctest/ctest.so") #|| File.exist?("lib/ctest/ctest.bundle")
  # Development
  require "ctest/ctest.so"
else
  # Gem
  require "ctest.so"
end