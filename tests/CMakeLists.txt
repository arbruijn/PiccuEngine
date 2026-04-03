enable_testing()

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

target_compile_definitions(msafe_callfunction_golden_test PRIVATE MONO)
target_compile_definitions(value_function_golden_test PRIVATE MONO)
target_compile_definitions(osiris_event_golden_test PRIVATE MONO)
target_include_directories(msafe_callfunction_golden_test PRIVATE
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/Descent3
	${CMAKE_SOURCE_DIR}/scripts)
target_include_directories(value_function_golden_test PRIVATE
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/Descent3
	${CMAKE_SOURCE_DIR}/scripts)
target_include_directories(osiris_event_golden_test PRIVATE
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/lib
	${CMAKE_SOURCE_DIR}/Descent3
	${CMAKE_SOURCE_DIR}/scripts)

set(MSAFE_CALLFUNCTION_EXPECTED_LOG "${CMAKE_CURRENT_SOURCE_DIR}/msafe_callfunction_expected.log")
set(MSAFE_CALLFUNCTION_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/msafe_callfunction_actual.log")
set(VALUE_FUNCTION_EXPECTED_LOG "${CMAKE_CURRENT_SOURCE_DIR}/value_function_expected.log")
set(VALUE_FUNCTION_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/value_function_actual.log")
set(OSIRIS_EVENT_EXPECTED_LOG "${CMAKE_CURRENT_SOURCE_DIR}/osiris_event_expected.log")
set(OSIRIS_EVENT_ACTUAL_LOG "${CMAKE_CURRENT_BINARY_DIR}/osiris_event_actual.log")

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
	NAME osiris_event_golden
	COMMAND osiris_event_golden_test
		"${OSIRIS_EVENT_EXPECTED_LOG}"
		"${OSIRIS_EVENT_ACTUAL_LOG}")
