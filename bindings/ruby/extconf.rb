=begin
extconf.rb for Ruby/Liblicense bindings

Copyright 2007, Creative Commons, www.creativecommons.org
Copyright 2007, Alex Roberts
=end

require 'mkmf'

dir_config('liblicense')

unless have_library('license', 'll_get_jurisdiction') and have_header('liblicense.h')
	fail <<-EOM
  Can't find liblicense or liblicense.h
  
  Try passing --with-liblicense-dir or --with-liblicense-lib and --with-liblicense-include
  options to extconf.
  EOM
end

create_makefile('liblicense')

