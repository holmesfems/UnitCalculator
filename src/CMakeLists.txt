add_executable ( basicNumeric
	basicNumeric.cpp  basicNumeric.h
	regextool.cpp  regextool.h
	stringTool.cpp  stringTool.h
	main.cpp
	)

# TODO check if compiler support c++11 standard
set_property (TARGET basicNumeric PROPERTY CXX_STANDARD 11)

target_link_libraries ( basicNumeric ${LIB_READLINE} )

install (TARGETS basicNumeric
	RUNTIME DESTINATION  bin
	)
