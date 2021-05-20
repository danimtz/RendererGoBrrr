set(PROJECT_NAME SRGB)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

################################################################################
# Source groups
################################################################################

if(WIN32)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(SDL2_PATH "${CMAKE_SOURCE_DIR}/SRGB/vendor/SDL2")
    endif()
endif()

find_package(OpenMP REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIR} 
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/stbimage"
)


file(GLOB ALL_FILES "*.h" "*.cpp" "src/*.cpp" "src/*.h" "vendor/stbimage/*.h")

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

#set(ROOT_NAMESPACE SRGB)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})


################################################################################
# Output directory
################################################################################
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-${CMAKE_VS_PLATFORM_NAME}/${PROJECT_NAME}/"
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
    
    #Copy sdl2 dll file
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    ${SDL2_DLL}
    $<TARGET_FILE_DIR:${PROJECT_NAME}>)


endif()


################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "_CONSOLE;"
    "UNICODE;"
    "_UNICODE"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:
            /Oi;
            /Gy
        >
        /permissive-;
        /std:c++17;
        /sdl;
        /W3;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        /openmp
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /INCREMENTAL
        >
        $<$<CONFIG:Release>:
            /OPT:REF;
            /OPT:ICF;
            /INCREMENTAL:NO
        >
        /DEBUG;
        /SUBSYSTEM:CONSOLE
    )
endif()





