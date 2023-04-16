/**
 * @version 1.0
 * @date 2023-04-16
 * 
 * @copyright Copyright (c) 2023
 * 			  Please see LICENSE For more detail 
 */
#include <common/common.h>
#include <common/logger.h>
#include <common/args_parser.h>
#include <syntax/syntax.h>


int main(int argc , char* argv[]) 
{
	SYS_INFO("\nWelcome to Pascal2C Compiler System!\n");


	/* ------------------------------------------------ */
	SYS_INFO("\nStep 0 : Initialize Arguments...\n");
	
	ArgsParser::ArgsInfo info = ArgsParser::parser(argc , argv);

	if (info.source_file.empty()) {
		ERROR("P2C" ,0 ,"Fatal : no input files\n");
		return EXIT_FAILURE;
	} 

	/* ------------------------------------------------ */
	SYS_INFO("Step 1 : Lexer Analysis\n");



	/* ------------------------------------------------ */
	SYS_INFO("Step 2 : Syntax Analysis\n");

	Pascal2C::Syntax::Parser syn{info.source_file , nullptr};

	return EXIT_SUCCESS;
}