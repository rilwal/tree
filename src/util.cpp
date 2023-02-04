
#include "util.hpp"

#include <cstdio>

std::vector<uint8_t> load_file(std::string filename, size_t offset, size_t len) {
	 FILE* file;
	 std::vector<uint8_t> contents;

	#ifdef _WIN32
	 errno_t error = fopen_s(&file, filename.c_str(), "rb");

	 if (error != 0) {
		 fprintf(stderr, "Failed to open file %s.\n", filename.c_str());
		 return contents;
	 }
	#else
	 file = fopen(filename.c_str(), "r");
	 if (!file) {
		 fprintf(stderr, "Failed to open file %s.\n", filename.c_str());
		 return contents;
	 }
	#endif
	 fseek(file, 0, SEEK_END);
	 size_t filesize = ftell(file);
	 rewind(file);

	 if (len == -1) len = filesize;

	 contents.resize(len + 1);

	 fseek(file, offset, SEEK_CUR);

	 fread(contents.data(), 1, len, file);
	 fclose(file);

	 contents[len] = '\0';

	 return contents;
}
