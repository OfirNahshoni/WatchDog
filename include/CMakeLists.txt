# include/CMakeLists.txt

# libraries
add_library(uid_lib INTERFACE)
add_library(task_lib INTERFACE)
add_library(heap_lib INTERFACE)
add_library(dvector_lib INTERFACE)
add_library(heap_p_queue_lib INTERFACE)
add_library(heap_scheduler_lib INTERFACE)
add_library(watch_dog_lib INTERFACE)    # private lib
add_library(wd_lib INTERFACE)           # public lib

# include libraries
target_include_directories(uid_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(task_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(heap_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(dvector_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(heap_p_queue_lib INTERFACE
                                                    ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(heap_scheduler_lib INTERFACE
                                                    ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(watch_dog_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(wd_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# link libraries

