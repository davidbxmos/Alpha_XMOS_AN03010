# If venv not defined, error
if(NOT DEFINED VENV_DIR)
    message(FATAL_ERROR "VENV_DIR is not defined.")
endif()

# Find the Python executable
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(PYTHON_IN_VENV "${VENV_DIR}/Scripts/python.exe")
else()
    set(PYTHON_IN_VENV "${VENV_DIR}/bin/python")
endif()

set(MARKER_FILE ${VENV_DIR}/.setup_done)
if(NOT EXISTS ${MARKER_FILE})
    message(STATUS "Setting up Python virtual environment...")

    # Create the virtual environment
    find_package(Python3 3.12 REQUIRED COMPONENTS Interpreter)
    execute_process(
        COMMAND ${Python3_EXECUTABLE} -m venv ${VENV_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND_ERROR_IS_FATAL ANY    
    )

    # Install requirements
    execute_process(
        COMMAND ${PYTHON_IN_VENV} -m pip install -r ../requirements.txt
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )

    # Create marker file
    file(WRITE ${MARKER_FILE} "done")
endif()

# -------------------------------------------------- 
# Export model (adjust PATH to enable xcore-opt)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(ENV{PATH} "${VENV_DIR}/bin:$ENV{PATH}")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(ENV{PATH} "${VENV_DIR}/bin:$ENV{PATH}")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(ENV{PATH} "${VENV_DIR}/Scripts;$ENV{PATH}")
endif()


message(STATUS ${PYTHON_IN_VENV})
message(STATUS "execute_process... kws")
execute_process(
    COMMAND ${PYTHON_IN_VENV} src/kwd_spotter/model/export.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE result
    ERROR_VARIABLE err
)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Python script failed:\n${err}")
endif()

message(STATUS "execute_process... vnr")
execute_process(
    COMMAND ${PYTHON_IN_VENV} src/vnr/model/export.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    RESULT_VARIABLE result
    ERROR_VARIABLE err
)
if(NOT result EQUAL 0)
    message(FATAL_ERROR "Python script failed:\n${err}")
endif()
