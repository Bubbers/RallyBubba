
# Files to compile
set(GAME_SOURCE_FILES main.cpp controls.h)

# Subdirectories
add_subdirectory(cameras)
add_subdirectory(components)
add_subdirectory(ui)
add_subdirectory(objects)

# Define the executable
set(EXECUTABLE_NAME HallsOfBubba)
add_executable(${EXECUTABLE_NAME} ${GAME_SOURCE_FILES} ${GAME_HEADER_FILES})
target_link_libraries(${EXECUTABLE_NAME} LINK_PUBLIC Bubba3D)
