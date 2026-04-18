enable_testing()

set(PICCUENGINE_TEST_INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/Descent3
	${CMAKE_SOURCE_DIR}/scripts)

add_executable(msafe_callfunction_golden_test
	msafe_callfunction_golden_test.cpp
	msafe_callfunction_test.cpp
	msafe_callfunction_callers.cpp)

add_executable(value_function_golden_test
	value_function_golden_test.cpp
	value_function_test.cpp
	value_function_callers.cpp)

add_executable(osiris_event_golden_test
	osiris_event_golden_test.cpp
	osiris_event_test.cpp
	osiris_event_callers.cpp)
add_executable(osiris_import_golden_test
	osiris_import_golden_test.cpp
	osiris_import_host.cpp
	osiris_import_runtime.cpp)

target_compile_definitions(msafe_callfunction_golden_test PRIVATE MONO)
target_compile_definitions(value_function_golden_test PRIVATE MONO)
target_compile_definitions(osiris_event_golden_test PRIVATE MONO)
target_compile_definitions(osiris_import_golden_test PRIVATE MONO)
target_include_directories(msafe_callfunction_golden_test PRIVATE
	${PICCUENGINE_TEST_INCLUDE_DIRS})
target_include_directories(value_function_golden_test PRIVATE
	${PICCUENGINE_TEST_INCLUDE_DIRS})
target_include_directories(osiris_event_golden_test PRIVATE
	${PICCUENGINE_TEST_INCLUDE_DIRS})
target_include_directories(osiris_import_golden_test PRIVATE
	${PICCUENGINE_TEST_INCLUDE_DIRS})

include(FetchContent)
set(BUILD_TESTS OFF CACHE BOOL "Disable emu86 self-tests when vendored into PiccuEngine" FORCE)
FetchContent_Declare(emu86
	SOURCE_DIR "${CMAKE_SOURCE_DIR}/../emu86")
FetchContent_MakeAvailable(emu86)

find_program(OWCC_EXECUTABLE owcc REQUIRED)

set(MSAFE_CALLFUNCTION_EMU86_DLL_DIR "${CMAKE_CURRENT_BINARY_DIR}/dll")
set(MSAFE_CALLFUNCTION_EMU86_DLL "${MSAFE_CALLFUNCTION_EMU86_DLL_DIR}/msafe_callfunction_callers.dll")
set(OSIRIS_EVENT_EMU86_DLL "${MSAFE_CALLFUNCTION_EMU86_DLL_DIR}/osiris_event_callevent.dll")
set(OSIRIS_IMPORT_EMU86_DLL "${MSAFE_CALLFUNCTION_EMU86_DLL_DIR}/osiris_import_runtime.dll")
file(MAKE_DIRECTORY "${MSAFE_CALLFUNCTION_EMU86_DLL_DIR}")

add_custom_command(
	OUTPUT "${MSAFE_CALLFUNCTION_EMU86_DLL}"
	COMMAND "${OWCC_EXECUTABLE}" -b nt -shared -x c++ -g
		-DMSAFE_CALLFUNCTION_IS_POINTER
		-I"${CMAKE_SOURCE_DIR}"
		-I"${CMAKE_SOURCE_DIR}/lib"
		-I"${CMAKE_SOURCE_DIR}/Descent3"
		-I"${CMAKE_SOURCE_DIR}/scripts"
		-o "${MSAFE_CALLFUNCTION_EMU86_DLL}"
		"${CMAKE_CURRENT_SOURCE_DIR}/msafe_callfunction_callers.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/msafe_callfunction_emu86_dll.cpp"
	DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/msafe_callfunction_callers.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/msafe_callfunction_callers.h"
		"${CMAKE_CURRENT_SOURCE_DIR}/msafe_callfunction_emu86_dll.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/msafe_callfunction_test.h"
		"${CMAKE_SOURCE_DIR}/Descent3/osiris_share.h"
		"${CMAKE_SOURCE_DIR}/scripts/osiris_common.h"
	VERBATIM)

add_custom_target(msafe_callfunction_emu86_dll
	DEPENDS "${MSAFE_CALLFUNCTION_EMU86_DLL}")

add_custom_command(
	OUTPUT "${OSIRIS_EVENT_EMU86_DLL}"
	COMMAND "${OWCC_EXECUTABLE}" -b nt -shared -x c++ -g
		-DOSIRIS_EVENT_USE_LOGGER_CALLBACK
		-I"${CMAKE_SOURCE_DIR}"
		-I"${CMAKE_SOURCE_DIR}/lib"
		-I"${CMAKE_SOURCE_DIR}/Descent3"
		-I"${CMAKE_SOURCE_DIR}/scripts"
		-o "${OSIRIS_EVENT_EMU86_DLL}"
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_event_test.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_event_emu86_dll.cpp"
	DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_event_test.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_event_test.h"
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_event_emu86_dll.cpp"
		"${CMAKE_SOURCE_DIR}/scripts/osiris_common.h"
	VERBATIM)

add_custom_target(osiris_event_emu86_dll
	DEPENDS "${OSIRIS_EVENT_EMU86_DLL}")

add_custom_command(
	OUTPUT "${OSIRIS_IMPORT_EMU86_DLL}"
	COMMAND "${OWCC_EXECUTABLE}" -b nt -shared -x c++ -g
		-I"${CMAKE_SOURCE_DIR}"
		-I"${CMAKE_SOURCE_DIR}/lib"
		-I"${CMAKE_SOURCE_DIR}/Descent3"
		-I"${CMAKE_SOURCE_DIR}/scripts"
		-o "${OSIRIS_IMPORT_EMU86_DLL}"
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_import_runtime.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_import_emu86_dll.cpp"
	DEPENDS
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_import_runtime.cpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_import_runtime.h"
		"${CMAKE_CURRENT_SOURCE_DIR}/osiris_import_emu86_dll.cpp"
		"${CMAKE_SOURCE_DIR}/Descent3/osiris_imports_shared.h"
		"${CMAKE_SOURCE_DIR}/scripts/osiris_import.h"
		"${CMAKE_SOURCE_DIR}/scripts/osiris_common.h"
	VERBATIM)

add_custom_target(osiris_import_emu86_dll
	DEPENDS "${OSIRIS_IMPORT_EMU86_DLL}")

add_executable(msafe_callfunction_emu86_test
	msafe_callfunction_emu86_test.cpp
	msafe_callfunction_test.cpp
	${CMAKE_SOURCE_DIR}/emuabi/msafe.cpp)
add_executable(osiris_event_emu86_test
	osiris_event_emu86_test.cpp
	osiris_event_callers.cpp
	${CMAKE_SOURCE_DIR}/emuabi/eventinfo.cpp)
add_executable(osiris_import_emu86_test
	osiris_import_emu86_test.cpp
	osiris_import_host.cpp)
target_compile_definitions(msafe_callfunction_emu86_test PRIVATE MONO)
target_compile_definitions(osiris_event_emu86_test PRIVATE MONO)
target_compile_definitions(osiris_import_emu86_test PRIVATE MONO)
target_include_directories(msafe_callfunction_emu86_test PRIVATE
	${PICCUENGINE_TEST_INCLUDE_DIRS}
	${emu86_SOURCE_DIR}/src
	${emu86_SOURCE_DIR}/src/cpu
	${emu86_SOURCE_DIR}/src/cpu/include
	${emu86_SOURCE_DIR}/include)
target_include_directories(osiris_event_emu86_test PRIVATE
	${PICCUENGINE_TEST_INCLUDE_DIRS}
	${emu86_SOURCE_DIR}/src
	${emu86_SOURCE_DIR}/src/cpu
	${emu86_SOURCE_DIR}/src/cpu/include
	${emu86_SOURCE_DIR}/include)
target_include_directories(osiris_import_golden_test PRIVATE
	${emu86_SOURCE_DIR}/src
	${emu86_SOURCE_DIR}/src/cpu
	${emu86_SOURCE_DIR}/src/cpu/include
	${emu86_SOURCE_DIR}/include)
target_include_directories(osiris_import_emu86_test PRIVATE
	${PICCUENGINE_TEST_INCLUDE_DIRS}
	${emu86_SOURCE_DIR}/src
	${emu86_SOURCE_DIR}/src/cpu
	${emu86_SOURCE_DIR}/src/cpu/include
	${emu86_SOURCE_DIR}/include)
target_link_libraries(msafe_callfunction_emu86_test PRIVATE emu86)
target_link_libraries(osiris_event_emu86_test PRIVATE emu86)
target_link_libraries(osiris_import_emu86_test PRIVATE emu86)
add_dependencies(msafe_callfunction_emu86_test msafe_callfunction_emu86_dll)
add_dependencies(osiris_event_emu86_test osiris_event_emu86_dll)
add_dependencies(osiris_import_emu86_test osiris_import_emu86_dll)

set(MSAFE_CALLFUNCTION_EXPECTED_LOG "${CMAKE_CURRENT_SOURCE_DIR}/msafe_callfunction_expected.log")
set(MSAFE_CALLFUNCTION_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/msafe_callfunction_actual.log")
set(MSAFE_CALLFUNCTION_EMU86_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/msafe_callfunction_emu86_actual.log")
set(VALUE_FUNCTION_EXPECTED_LOG "${CMAKE_CURRENT_SOURCE_DIR}/value_function_expected.log")
set(VALUE_FUNCTION_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/value_function_actual.log")
set(OSIRIS_EVENT_EXPECTED_LOG "${CMAKE_CURRENT_SOURCE_DIR}/osiris_event_expected.log")
set(OSIRIS_EVENT_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/osiris_event_actual.log")
set(OSIRIS_EVENT_EMU86_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/osiris_event_emu86_actual.log")
set(OSIRIS_IMPORT_EXPECTED_LOG "${CMAKE_CURRENT_SOURCE_DIR}/osiris_import_expected.log")
set(OSIRIS_IMPORT_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/osiris_import_actual.log")
set(OSIRIS_IMPORT_EMU86_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/osiris_import_emu86_actual.log")

add_test(
	NAME msafe_callfunction_golden
	COMMAND msafe_callfunction_golden_test
		"${MSAFE_CALLFUNCTION_EXPECTED_LOG}"
		"${MSAFE_CALLFUNCTION_ACTUAL_LOG}")

add_test(
	NAME value_function_golden
	COMMAND value_function_golden_test
		"${VALUE_FUNCTION_EXPECTED_LOG}"
		"${VALUE_FUNCTION_ACTUAL_LOG}")

add_test(
	NAME msafe_callfunction_emu86
	COMMAND msafe_callfunction_emu86_test
		"${MSAFE_CALLFUNCTION_EXPECTED_LOG}"
		"${MSAFE_CALLFUNCTION_EMU86_ACTUAL_LOG}"
		"${MSAFE_CALLFUNCTION_EMU86_DLL}")

add_test(
	NAME osiris_event_golden
	COMMAND osiris_event_golden_test
		"${OSIRIS_EVENT_EXPECTED_LOG}"
		"${OSIRIS_EVENT_ACTUAL_LOG}")

add_test(
	NAME osiris_import_golden
	COMMAND osiris_import_golden_test
		"${OSIRIS_IMPORT_EXPECTED_LOG}"
		"${OSIRIS_IMPORT_ACTUAL_LOG}")

add_test(
	NAME osiris_event_emu86
	COMMAND osiris_event_emu86_test
		"${OSIRIS_EVENT_EXPECTED_LOG}"
		"${OSIRIS_EVENT_EMU86_ACTUAL_LOG}"
		"${OSIRIS_EVENT_EMU86_DLL}")

add_test(
	NAME osiris_import_emu86
	COMMAND osiris_import_emu86_test
		"${OSIRIS_IMPORT_EXPECTED_LOG}"
		"${OSIRIS_IMPORT_EMU86_ACTUAL_LOG}"
		"${OSIRIS_IMPORT_EMU86_DLL}")
