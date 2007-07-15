require 'liblicense.so'

class Liblicense

	def to_s
		self.name + " " + self.version.join('.') + " (" + self.jurisdiction + ")"
	end

end
