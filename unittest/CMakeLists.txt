

aux_source_directory(. Unit_Src)

add_executable(test_all ${Unit_Src})

target_link_libraries(test_all json)


