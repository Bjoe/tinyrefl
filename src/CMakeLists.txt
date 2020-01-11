add_library(tinyrefl utils/demangle.cpp)

if(HUNTER_ENABLED)
    target_link_libraries(tinyrefl PUBLIC ctti::ctti nlohmann_json::nlohmann_json)
else()
    target_link_libraries(tinyrefl PUBLIC tinyrefl_externals_ctti tinyrefl_externals_jsonformoderncpp)
endif()

target_include_directories(tinyrefl PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
define_tinyrefl_version_variables(tinyrefl)

try_compile(TINYREFL_HAS_CONSTEXPR_ARRAY_VIEW_SUBSCRIPT
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/check_constexpr_array_view_subscript.cpp"
    CMAKE_FLAGS -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
    OUTPUT_VARIABLE output)

if(TINYREFL_HAS_CONSTEXPR_ARRAY_VIEW_SUBSCRIPT)
    target_compile_definitions(tinyrefl PUBLIC TINYREFL_HAS_CONSTEXPR_ARRAY_VIEW_SUBSCRIPT)
else()
    message(WARNING "Constexpr array_view subscript operator not supported by the compiler. Build error: ${output}")
ENDIF()

target_compile_features(tinyrefl PUBLIC cxx_std_14)

# Introduce variables:
# * CMAKE_INSTALL_LIBDIR
# * CMAKE_INSTALL_BINDIR
# * CMAKE_INSTALL_INCLUDEDIR
include(GNUInstallDirs)

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/tinyrefl")
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/tinyreflConfigVersion.cmake")
set(project_config "${generated_dir}/tinyreflConfig.cmake")
set(TARGETS_EXPORT_NAME "tinyreflTargets")
set(namespace "tinyrefl::")

install(TARGETS tinyrefl
  EXPORT "${TARGETS_EXPORT_NAME}"
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY "${TINYREFL_INCLUDE_DIR}/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

include(CMakePackageConfigHelpers)

# Use:
# * PROJECT_VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Use:
# * TARGETS_EXPORT_NAME
# * PROJECT_NAME
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)
