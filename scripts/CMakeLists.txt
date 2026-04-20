
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_definitions(-DCHECKSUM=2273889867L)
else()
	add_definitions(-DCHECKSUM=2273873307UL -D__32BIT__)
endif()

SET (HEADERS )
SET (CPPS )
IF(UNIX)
SET (CPPS
	linux_lib.cpp)
ENDIF()

SET (SCRIPTS
	aigame2
	AIGame3
	aigame4
	AIGame
	barney
	BatteriesIncluded
	BossCamera
	CanyonsCTF
	CellTestLevel
	ChrisTest
	clutter
	GameGauge
	generic
	Geodomes
	HalfPipe
	InfernalBolt
	Inversion
	LEVEL0
	level10
	level11
	Level12
	level13
	level14
	LEVEL15
	Level16
	level17
	level1
	level2
	level3
	level4
	level5
	Level6
	level7
	level8
	Level9
	LevelS1
	levelS2
	Merc02
	Merc1
	Merc3
	Merc4
	merc5
	Merc6
	Merc7
	myPowerHouse
	Mysterious_Isle
	orbital
	Paranoia
	PiccuStation
	Polaris
	Quadsomniac
	RudeAwakening
	SewerRat
	testscript
	TrainingMission
	Y2K)

SET (SCRIPTTARGETS )

set (FULLDIR "${CMAKE_CURRENT_BINARY_DIR}/linuxfullhog")
file (MAKE_DIRECTORY "${FULLDIR}")

FOREACH( SCRIPT ${SCRIPTS})
 
  ADD_LIBRARY(${SCRIPT} SHARED ${CPPS} "${SCRIPT}.cpp")
  set_target_properties(${SCRIPT} PROPERTIES PREFIX "")
  list(APPEND SCRIPTTARGETS ${SCRIPT})
    IF(UNIX)
      set(SCRIPT_MODULE_SUFFIX ".so")
      if(APPLE)
        set(SCRIPT_MODULE_SUFFIX ".dylib")
      endif()
      ADD_CUSTOM_COMMAND(
	TARGET ${SCRIPT}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SCRIPT}> "${FULLDIR}/"
	COMMENT "Copying file $<TARGET_FILE:${SCRIPT}> to hogfile directory..."
	)
      #ADD_CUSTOM_COMMAND(
	#TARGET ${SCRIPT}
	#POST_BUILD
	#COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${SCRIPT}> "${CMAKE_SOURCE_DIR}/scripts/data/linuxdemohog/"
	#)

    ENDIF()

ENDFOREACH()

IF(UNIX)
  set(SCRIPT_MODULE_SUFFIX ".so")
  if(APPLE)
    set(SCRIPT_MODULE_SUFFIX ".dylib")
  endif()

  SET(HOGBUILDCMD "${CMAKE_SOURCE_DIR}/scripts/hogutils/hogUtils-i686")
  SET(HOGARCH "linux")
  IF(APPLE)
    SET(HOGBUILDCMD "${CMAKE_SOURCE_DIR}/scripts/hogutils/hogUtils-macosx")
    SET(HOGARCH "osx")
  ENDIF()

  set(FULL_HOG_MANIFEST "${CMAKE_CURRENT_BINARY_DIR}/d3linuxfullhog.txt")
  file(READ "${CMAKE_CURRENT_SOURCE_DIR}/data/linuxfullhog/d3linuxfullhog.txt" FULL_HOG_MANIFEST_CONTENT)
  if(SCRIPT_MODULE_SUFFIX STREQUAL ".so")
    string(REPLACE "polaris.so" "Polaris.so" FULL_HOG_MANIFEST_CONTENT "${FULL_HOG_MANIFEST_CONTENT}")
  endif()
  foreach(SCRIPT ${SCRIPTS})
    if(NOT FULL_HOG_MANIFEST_CONTENT MATCHES "(^|[\r\n])${SCRIPT}\\${SCRIPT_MODULE_SUFFIX}([\r\n]|$)")
      string(APPEND FULL_HOG_MANIFEST_CONTENT "${SCRIPT}${SCRIPT_MODULE_SUFFIX}\n")
    endif()
  endforeach()
  file(WRITE "${FULL_HOG_MANIFEST}" "${FULL_HOG_MANIFEST_CONTENT}")

  SET(FULLHOG "${FULLDIR}/d3-${HOGARCH}.hog")
  ADD_CUSTOM_COMMAND(
    OUTPUT "${FULLHOG}"
    COMMAND ${HOGBUILDCMD} "--makehog" "${FULL_HOG_MANIFEST}"
    COMMAND ${CMAKE_COMMAND} -E remove "${FULLHOG}"
    COMMAND ${CMAKE_COMMAND} -E copy "${FULLDIR}/new.hog" "${FULLHOG}"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${D3_GAMEDIR}"
    COMMAND ${CMAKE_COMMAND} -E copy "${FULLHOG}" "${D3_GAMEDIR}"
    COMMAND ${CMAKE_COMMAND} -E remove "${FULLDIR}/new.hog"
    WORKING_DIRECTORY "${FULLDIR}"
    DEPENDS ${SCRIPTTARGETS}
    COMMENT "Building platform specific hog."
    )
  ADD_CUSTOM_TARGET("FullLinuxHog" ALL DEPENDS "${FULLHOG}")

  #SET(DEMOHOG "${DEMODIR}/d3-${HOGARCH}.hog")
  #SET(DEMO_HOG_MANIFEST "${CMAKE_SOURCE_DIR}/scripts/data/linuxdemohog/"d3linuxdemohog.txt")
  #ADD_CUSTOM_COMMAND(
  #  OUTPUT "${DEMOHOG}"
  #  COMMAND ${HOGBUILDCMD} "--makehog" "${DEMO_HOG_MANIFEST}"
  #  COMMAND ${CMAKE_COMMAND} -E remove "${DEMOHOG}"
  #  COMMAND ${CMAKE_COMMAND} -E copy "${DEMODIR}/new.hog" "${DEMOHOG}"
  #  COMMAND ${CMAKE_COMMAND} -E make_directory "${D3_GAMEDIR}"
  #  COMMAND ${CMAKE_COMMAND} -E copy "${DEMOHOG}" "${D3_GAMEDIR}"
  #  COMMAND ${CMAKE_COMMAND} -E remove "${DEMODIR}/new.hog"
  #  WORKING_DIRECTORY "${DEMODIR}"
  #  DEPENDS ${SCRIPTTARGETS}
  #  COMMENT "Building platform specific hog."
  #  )
  #ADD_CUSTOM_TARGET("DemoLinuxHog" ALL DEPENDS "${DEMOHOG}")
ENDIF()
