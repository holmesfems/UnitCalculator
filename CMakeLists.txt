cmake_minimum_required (VERSION 2.8)

project (UnitCalculator)

find_library ( LIB_READLINE
	NAMES readline libreadline
	)

add_subdirectory (src)
