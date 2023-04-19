
# Add all valid file in common/
Add_Library(common STATIC
	args_parser.cc
	args_parser.h
	common_type.h
	common.h
	logger.h
)

# Include Gflags
include(FetchContent)

FetchContent_Declare(
    gflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    GIT_TAG v2.2.2
    )
FetchContent_MakeAvailable(gflags)

Target_Link_Libraries(common PRIVATE
	gflags::gflags
)