cmake_minimum_required(VERSION "4.0.2")

set(CMAKE_CXX_STANDARD 17)

#Set this to true for shipping!
set(PRODUCTION_BUILD FALSE CACHE BOOL "Make this a production build" FORCE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")

if(PRODUCTION_BUILD)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimisation
else()
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

if(MSVC)
add_compile_options(/arch:AVX2) #Optimises some stuff... idk
endif()


#Main project
project(main)

#Compile shaders in pre-build
set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders/")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources/shaders_bin/")
file(MAKE_DIRECTORY ${SHADER_BINARY_DIR})
file(GLOB_RECURSE SHADER_FILES
    "${SHADER_SOURCE_DIR}/*.vert"
    "${SHADER_SOURCE_DIR}/*.frag"
)
set(COMPILED_SHADERS "")

foreach(SHADER ${SHADER_FILES})
    get_filename_component(FILE_NAME ${SHADER} NAME)
    set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")

    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND glslangValidator -V ${SHADER} -o ${SPIRV}
        DEPENDS ${SHADER}
        COMMENT "Compiling shader ${FILE_NAME} to SPIR-V"
        VERBATIM
    )

    list(APPEND COMPILED_SHADERS ${SPIRV})
endforeach()

add_custom_target(compile_shaders ALL
    DEPENDS ${COMPILED_SHADERS}
)




#Add glm
add_subdirectory(thirdParty/glm)

# GLFW static config
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
add_subdirectory(thirdParty/glfw)

# Vulkan SDK
find_package(Vulkan REQUIRED)

# Executable and linking
# We create MY_SOURCES, a list of all the source files of the project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(main "${MY_SOURCES}")
target_include_directories(main PRIVATE "CMAKE_CURRENT_SOURCE_DIR/include/")
target_link_libraries(main PRIVATE glfw Vulkan::Vulkan)
add_dependencies(main compile_shaders)

if(UNIX AND NOT APPLE)
    target_link_libraries(main PRIVATE dl pthread X11 Xrandr Xi Xxf86vm Xcursor)
endif()

# We make the RESOURCES_PATH constant that sends to the resources folder
if(PRODUCTION_BUILD)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") #Relative path
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1)

else()
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/") #Absolute path... ONLY DURING DEVELOPEMENT!
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)

endif()

if(MSVC)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS) #Remove useless warnings
endif()

target_include_directories(main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_link_libraries(main PRIVATE glm)