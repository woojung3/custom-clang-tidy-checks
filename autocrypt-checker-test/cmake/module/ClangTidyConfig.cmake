set(AUTOCRYPT_CLANG_TIDY_FLAGS
"-*,\
autocrypt*")

function(set_global_clang_flags targetname)
  set_target_properties(
    ${targetname}
    PROPERTIES
    CXX_CLANG_TIDY
"${CLANG_TIDY};-checks=-*,\
${AUTOCRYPT_CLANG_TIDY_FLAGS};\
-warnings-as-errors=-*,\
${AUTOCRYPT_CLANG_TIDY_FLAGS}"
  )
endfunction(set_global_clang_flags targetname)

function(get_all_targets _result _dir)
  get_property(_subdirs DIRECTORY "${_dir}" PROPERTY SUBDIRECTORIES)

  foreach(_subdir IN LISTS _subdirs)
    get_all_targets(${_result} "${_subdir}")
  endforeach()

  get_property(_sub_targets DIRECTORY "${_dir}" PROPERTY BUILDSYSTEM_TARGETS)
  set(${_result} ${${_result}} ${_sub_targets} PARENT_SCOPE)
endfunction()

function(add_subdirectory_with_static_analysis _folder)
  add_subdirectory(${_folder} ${ARGN})
  get_all_targets(_targets "${_folder}")

    foreach(_target IN LISTS _targets)
      set_global_clang_flags("${_target}")
      message(STATUS "Setting clang-tidy properties of target: ${_target}")
    endforeach()
endfunction()
