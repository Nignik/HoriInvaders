file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_executable(HoriInvaders ${SOURCES})

find_package(HECS)
find_package(imgui CONFIG REQUIRED)
find_package(HoriEngine)
find_package(glad REQUIRED)
find_package(glm CONFIG REQUIRED)

target_link_libraries(HoriInvaders PRIVATE HECS::HECS HoriEngine::HoriEngine)