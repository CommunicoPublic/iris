# RagelGenerate.cmake
#
# Generate parser
#
FIND_PROGRAM(RAGEL NAMES ragel PATHS /usr/bin /usr/local/bin /usr/pkg/bin /opt/csw/bin)
IF(RAGEL)
    MESSAGE(STATUS "Ragel found at: ${RAGEL}")
ELSE(RAGEL)
    MESSAGE(FATAL_ERROR "Ragel not found")
ENDIF(RAGEL)

MACRO(RAGEL_GENERATE INPUT_FILE OUTPUT_FILE)

    MESSAGE(STATUS "Generating: ${INPUT_FILE} -> ${OUTPUT_FILE}")
    EXECUTE_PROCESS(COMMAND ${RAGEL} -e -o ${OUTPUT_FILE} ${INPUT_FILE})

ENDMACRO(RAGEL_GENERATE)
# End.
