require 'liblicense.so'

class Liblicense

	def to_s
		version = ""
		if self.version then
			version = " " + self.version.join('.')
		end
		self.name + version  + " (" + self.jurisdiction + ")"
	end

end
