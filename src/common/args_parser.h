#pragma once

#include <vector>
#include <string>

#include "common_type.h"


namespace ArgsParser{

class ArgsInfo {
public :
	std::vector<std::string> source_files;
	std::string target_name;
	u8 opt_level;
};

ArgsInfo parser(int argc , char* argv[]);
}