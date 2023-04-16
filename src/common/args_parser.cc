#include "args_parser.h"
#include "logger.h"
#include "common.h"

#include <iostream>
#include <getopt.h>

namespace ArgsParser {

static const char *optstring =
	"o:O:v";


static struct option long_options[] =
{
	{"help" 	, no_argument 	, nullptr , 'h'} ,
	{"version"	, no_argument 	, nullptr , 'v'} ,
	{nullptr 	, 0				, nullptr , 0}
};

static void showInvalid(const char* arg) {
	::printf("Invalid Argument : %s\n" , arg);
}

static void showVersion() {
	::printf("[ Pascal2C ] Compiler System Version : %d\n" , __P2C_VERSION__);
	::printf("For more details , pleace click %s\n" ,__GIT_URL__);
	::printf("                 @ BUPT CS 2023 Spring");

}

static void showHelp() {
	TODO;
}


ArgsInfo parser(int argc , char* argv[]) 
{
	i8 opt = 0;
	ArgsInfo info {};
	while ((opt = ::getopt_long(
								argc , 
								argv ,
								optstring ,
								long_options ,
								nullptr
	)) != -1)
	{
		switch (opt)
		{
		case 'o': info.target_name 	= optarg; 		break;
		case 'O': info.opt_level	= atoi(optarg); break;
		case 'v': showVersion();					break;
		case 'h': showHelp();						break;
		default : showInvalid(optarg);				break;
		}
	}
	
	return info;
}

}