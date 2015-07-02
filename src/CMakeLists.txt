include_directories(
	${CMAKE_SOURCE_DIR}/include
)

add_executable(DumpJump main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Game/Game.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Map/MapMgr.cpp)
target_link_libraries(DumpJump ${SFML_LIBRARIES})

