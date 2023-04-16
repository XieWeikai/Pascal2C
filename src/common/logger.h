#pragma once
/**
 * Useful Logging macros for project building
 * 
 * c-like style , e.g.
 * 		DEBUG("Dump data : %d\n" ,15);
 * 
 * 
 * 这里设置了两类信息，一类是用于目标文件的信息；
 * 	有三个等级
 * 		INFO
 * 		WARN
 * 		ERROR
 * 
 * 另一类用于编译器项目本身，可以在CMakeLists
 * 里配置关闭，有下面五个等级
 * 		SYS_INFO
 * 		SYS_WARN
 * 		SYS_ERROR
 * 		DEBUG
 * 		ASSERT
 */
#include <cstring>

#define COLOR_RED 	"\033[31m"
#define COLOR_GREE 	"\033[32m"
#define COLOR_YELL 	"\033[33m"
#define COLOR_BLUE	"\033[34m"
#define COLOR_END 	"\033[0m"

/**
 * @brief 给出更好的文件路径格式
 */
#ifdef WIN32
#define TRIM(x) (strrchr(x,'\\') ? strrchr(x,'\\') + 1 : x)
#else
#define TRIM(x) (strrchr(x, '/') ? strrchr(x, '/') + 1 : x)
#endif
/**
 * @brief 输出带有console颜色的字符串
 */
#define COLOR_STR(color ,str) color str COLOR_END

#define LOGGER(file ,line ,color ,level ,str ,...) do {	\
	fprintf(stderr, COLOR_STR(color , "["#level " @%s:%d]\t")	\
			str, file, line, ##__VA_ARGS__);	\
} while (0)	

/**
 * @brief 给出一条提示信息，标记的颜色为绿色
 * @param file 提示来源的文件名 %s
 * @param line 提示来源的行号 %d
 * @param str  格式化字符串
 */
#define INFO(file ,line ,str ,...) \
	LOGGER(file , line , COLOR_GREE ,INFO	, str , ##__VA_ARGS__)

/**
 * @brief 给出一条警告信息，标记的颜色为黄色
 * @param file 警告来源的文件名 %s
 * @param line 警告来源的行号 %d
 * @param str  格式化字符串
 */
#define WARN(file ,line ,str ,...) \
	LOGGER(file , line , COLOR_YELL ,WARNING , str , ##__VA_ARGS__)

/**
 * @brief 给出一条错误信息，标记的颜色为红色
 * @param file 错误来源的文件名 %s
 * @param line 错误来源的行号 %d
 * @param str  格式化字符串
 */
#define ERROR(file ,line ,str ,...) \
	LOGGER(file , line , COLOR_RED  ,ERROR   , str , ##__VA_ARGS__)


/* ----------------------------------------------------- */
#ifdef USE_SYS_LOGGER

/**
 * @brief 给出一条系统提示信息，标记的颜色为绿色
 * @param str  格式化字符串
 */
#define SYS_INFO(str ,...) \
	LOGGER(TRIM(__FILE__), __LINE__, COLOR_GREE, INFO, str, ##__VA_ARGS__)

/**
 * @brief 给出一条系统警告信息，标记的颜色为黄色
 * @param str  格式化字符串
 */
#define SYS_WARN(str ,...) \
	LOGGER(TRIM(__FILE__), __LINE__, COLOR_YELL, WARN, str, ##__VA_ARGS__)

/**
 * @brief 给出一条系统警告信息，标记的颜色为红色
 * @param str  格式化字符串
 */
#define SYS_ERROR(str ,...) \
	LOGGER(TRIM(__FILE__), __LINE__, COLOR_RED, ERROR, str, ##__VA_ARGS__)


/**
 * @brief 给出一条调试信息，标记的颜色为蓝色
 * @param str  格式化字符串
 */
#define DEBUG(str ,...) \
	LOGGER(TRIM(__FILE__), __LINE__, COLOR_BLUE,DEBUG, str, ##__VA_ARGS__)

/**
 * @brief 附加信息的断言
 * @param cond 断言条件
 * @param str 格式化字符串
 */
#define ASSERT(cond ,str ,...) do { 	\
	if (!(cond)) {						\
		SYS_ERROR(str , ##__VA_ARGS__);	\
		exit(EXIT_FAILURE); }			\
} while(0)


#else
#define SYS_INFO(str , ...)
#define SYS_WARN(str , ...)
#define SYS_ERROR(str, ...)
#define DEBUG(str , ...)
#define ASSERT(cond , msg)

#endif
/**
 * @brief 暂未实现的功能，使用将会触发断言
 */
#define TODO ASSERT(false , "****TODO****")