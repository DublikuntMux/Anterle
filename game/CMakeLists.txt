project(
  AnterleGame
  VERSION 1.0.3
  LANGUAGES CXX)

file(GLOB_RECURSE game_sources CONFIGURE_DEPENDS "./src/*.cpp")

add_executable(AnterleGame ${game_sources})
target_include_directories(AnterleGame PUBLIC include/)

set_target_properties(AnterleGame PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(AnterleGame PROPERTIES SOVERSION 1)

target_link_libraries(AnterleGame PRIVATE AnterleEngine)
target_include_directories(AnterleGame PRIVATE AnterleEngine)

add_custom_command(
  TARGET AnterleGame
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E echo "Running post build asset converting!"
  COMMAND $<TARGET_FILE:AnterleTool> ${CMAKE_SOURCE_DIR}/engine/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
  COMMAND $<TARGET_FILE:AnterleTool> ${CMAKE_SOURCE_DIR}/game/resources ${CMAKE_CURRENT_BINARY_DIR}/resources
  )
