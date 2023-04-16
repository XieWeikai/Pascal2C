include_directories(.)

find_package(FLEX REQUIRED)
find_package(Python3 COMPONENTS Interpreter REQUIRED)
set(SCRIPT_PATH 	${CMAKE_CURRENT_SOURCE_DIR}/token_generator.py)
set(OUTPUT_HEADER 	${CMAKE_CURRENT_BINARY_DIR}/lexer.h)
add_custom_command(
        OUTPUT  ${OUTPUT_HEADER}
        COMMAND ${Python3_EXECUTABLE} ${SCRIPT_PATH} ${OUTPUT_HEADER}
        DEPENDS ${SCRIPT_PATH}
        COMMENT "Generating ${OUTPUT_HEADER}"
)
FLEX_TARGET(SCANNER lexer.lex ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.c)

add_library( lexer STATIC
	lexer.c
	utils.c
	${FLEX_SCANNER_OUTPUTS}
	${OUTPUT_HEADER}
)

target_include_directories(lexer PUBLIC ${CMAKE_CURRENT_BINARY_DIR})