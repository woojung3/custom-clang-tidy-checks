MESSAGE(STATUS "==========================================")
MESSAGE(STATUS "Cert - C/CPP")
MESSAGE(STATUS "==========================================")

file(GLOB DEPS CONFIGURE_DEPENDS "src/*")
set(ALL_DEPS ${ALL_DEPS} ${DEPS})

add_library(${PROJECT_NAME} STATIC
  ${ALL_DEPS}
)
add_library(autocrypt::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    include
  PRIVATE
    src
)
