#include "args_parser.h"
#include "logger.h"
#include "common.h"

#include <iostream>
#include <getopt.h>
#include <gflags/gflags.h>

namespace ArgsParser {

static const char *optstring =
	"o:O:v";


static struct option long_options[] =
{
	{"help" 	, no_argument 	, nullptr , 'h'} ,
	{"version"	, no_argument 	, nullptr , 'v'} ,
	{nullptr 	, 0				, nullptr , 0  }
};

static void showInvalid(const char* arg) {
	::printf("Invalid Argument : %s\n" , arg);
}

static void showVersion() {
	::printf("[ Pascal2C ] Compiler System Version : %s\n" , __P2C_VERSION__);
	::printf("For more details , pleace click %s\n" ,__GIT_URL__);
	::printf("                 @ BUPT CS 2023 Spring\n");

	::exit(EXIT_SUCCESS);
}

static void showHelp() {
	TODO;
}

static ArgsInfo useUnixOpt(int argc , char* argv[]) 
{
	TODO;
	i8  opt = 0;
	i32 opt_index = 0;
	ArgsInfo info {};
	DEBUG("Num : %d Cname : %s\n" ,argc ,argv[0]);

	while (optind != argc) {

		opt = ::getopt_long(
			argc ,
			argv ,
			optstring ,
			long_options ,
			&opt_index
		);

		printf("opt : %d\nopt_index : %d\noptind : %d\n" ,
				opt , opt_index , optind);
		if (opt == -1) break;



		switch (opt) {
		
		case  0 : /* Default ./Pascal2C */			break;
		case 'o': info.target_name 	= optarg; 		break;
		case 'O': info.opt_level	= atoi(optarg); break;
		case 'v': showVersion();					break;
		case 'h': showHelp();						break;
		case -1 : 
			DEBUG("Get a Source %d : %s\n" ,opt_index , argv[opt_index]);
			info.source_files.emplace_back(argv[opt_index]);
													break;
		default : showInvalid(optarg);				break;
		}
	}
	
	return info;
}

DEFINE_string(o , "target.c" , "Ouput file\n");
DEFINE_string(O , "0", "Optimizition level");
ArgsInfo parser(int argc , char* argv[]) 
{
	gflags::ParseCommandLineFlags(&argc ,&argv ,true);
	printf("Opt Level : %s\n" ,FLAGS_O.c_str());
}


// End of Namespace ArgsParser
}