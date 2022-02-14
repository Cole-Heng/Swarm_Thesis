cmake_minimum_required(VERSION 2.8.12)

# set up the two types of flags called by "cmake -DCMAKE_BUILD_TYPE=Debug" and cmake -DCMAKE_BUILD_TYPE=Release"
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
# Watch if you're using the C or CXX compiler...couldn't get Wall through until realized in C
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall")

# makes it so the BIN is installed locally
set(CMAKE_INSTALL_PREFIX .) 

project(BOIDS)

#Collect the source files
file(GLOB_RECURSE EXEC_SOURCES SRC/*.c)
file(GLOB_RECURSE EXEC_HEADERS SRC/*.h)

#Create the executable
add_executable(boids_sim ${EXEC_SOURCES} ${EXEC_HEADERS}) 

target_link_libraries(boids_sim m)

find_program(CTAGS ctags)
# CTAGs
if(CTAGS)
	# Add "tags" target and make my_project depending on this target.
	set_source_files_properties(tags PROPERTIES GENERATED true)
	add_custom_target(tags
	    	COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .
		${EXEC_SOURCES} ${EXEC_HEADERS})
	add_dependencies(boids_sim tags)
	message(STATUS "Found ctags!")
endif()

#add_custom_target(tags COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q . WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

install(DIRECTORY DESTINATION BIN)
install(DIRECTORY DESTINATION SANDBOX)
install(DIRECTORY DESTINATION TESTING)

install(TARGETS boids_sim DESTINATION BIN)
install(TARGETS boids_sim DESTINATION SANDBOX)
install(TARGETS boids_sim DESTINATION TESTING)

# moves the current .gdbinit to SANDBOX for playing with
install(FILES DEBUG/.gdbinit DESTINATION SANDBOX)
