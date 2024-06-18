cmake_minimum_required (VERSION 3.5..3.26)

# MSVC runtime library flags are selected by an abstraction, CMake >= 3.15
# This policy still need to be set even with cmake_minimum_required() command above.
if (POLICY CMP0091)
	if ((DEFINED ENABLE_STATIC_RUNTIME) AND (DEFINED CMAKE_MSVC_RUNTIME_LIBRARY))
		message (FATAL_ERROR "Both ENABLE_STATIC_RUNTIME and CMAKE_MSVC_RUNTIME_LIBRARY are set.")
		return ()
	endif ()

	if (CMAKE_VERSION VERSION_LESS 3.18)
		if (DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
			cmake_policy (SET CMP0091 NEW)
		else ()
			cmake_policy (SET CMP0091 OLD)
		endif ()
	endif ()
endif ()

option (ENABLE_EXTERNAL_LIBS "Enable FLAC, Vorbis, and Opus codecs" ON)
if (ENABLE_EXTERNAL_LIBS)
	list (APPEND VCPKG_MANIFEST_FEATURES "external-libs")
endif ()

option (ENABLE_MPEG "Enable MPEG codecs" ON)
if (ENABLE_MPEG)
	list (APPEND VCPKG_MANIFEST_FEATURES "mpeg")
endif ()

option (ENABLE_EXPERIMENTAL "Enable experimental code" OFF)
if (ENABLE_EXPERIMENTAL)
	list (APPEND VCPKG_MANIFEST_FEATURES "speex")
endif ()

option (BUILD_REGTEST "Build regtest" OFF)
if (BUILD_REGTEST)
	list (APPEND VCPKG_MANIFEST_FEATURES "regtest")
endif ()

project(libsndfile VERSION 1.2.2)

#
# Variables
#

set (CMAKE_C_STANDARD 99)
set (CMAKE_C_STANDARD_REQUIRED TRUE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (PACKAGE_NAME ${PROJECT_NAME})
set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set (CPACK_PACKAGE_VERSION_STAGE "")
set (CPACK_PACKAGE_VERSION_FULL "${PROJECT_VERSION}${CPACK_PACKAGE_VERSION_STAGE}")

#
# System-wide includes
#

include (GNUInstallDirs)
include (FeatureSummary)
include (CMakeDependentOption)
include (CTest)
include (CheckCCompilerFlag)

#
# Options
#

option (BUILD_SHARED_LIBS "Build shared libraries" OFF)

option (ENABLE_CPACK "Enable CPack support" ON)
if (MSVC AND (DEFINED ENABLE_STATIC_RUNTIME))
	option (ENABLE_STATIC_RUNTIME "Enable static runtime" ${ENABLE_STATIC_RUNTIME})
elseif (MINGW)
	option (ENABLE_STATIC_RUNTIME "Enable static runtime" OFF)
endif ()

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if (CMAKE_VERSION VERSION_LESS 3.14)
	list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/sqlite")
endif ()

#
# Setup definitions
#

include(SndFileChecks)

if (ENABLE_EXTERNAL_LIBS AND NOT (Vorbis_FOUND OR FLAC_FOUND OR OPUS_FOUND))
	set (ENABLE_EXTERNAL_LIBS OFF)
endif()
if(ENABLE_MPEG AND (NOT HAVE_MPEG_LIBS))
	set (ENABLE_MPEG OFF)
endif()
if (BUILD_REGTEST AND (NOT SQLITE3_FOUND))
	set (BUILD_REGTEST OFF)
endif()

cmake_dependent_option (ENABLE_COMPATIBLE_LIBSNDFILE_NAME "Set DLL name to libsndfile-1.dll (canonical name), sndfile.dll otherwise" OFF "WIN32;BUILD_SHARED_LIBS" OFF)

if (NOT MSVC)
	if (CPU_IS_X86)
		check_c_compiler_flag (-msse2 HAVE_MSSE2_COMPILER_FLAG)
		if (HAVE_MSSE2_COMPILER_FLAG)
			cmake_dependent_option (ENABLE_SSE2 "Add SSE2 compiler flag" ON "HAVE_MSSE2_COMPILER_FLAG" OFF)
		endif ()
	endif ()
endif ()
if (ENABLE_SSE2)
	add_compile_options (-msse2)
endif ()

set (HAVE_EXTERNAL_XIPH_LIBS ${ENABLE_EXTERNAL_LIBS})
set (HAVE_SQLITE3 ${BUILD_REGTEST})
set (HAVE_ALSA_ASOUNDLIB_H ${ALSA_FOUND})
set (HAVE_SNDIO_H ${SNDIO_FOUND})

set (ENABLE_EXPERIMENTAL_CODE ${ENABLE_EXPERIMENTAL})
set (HAVE_MPEG ${ENABLE_MPEG})
set (HAVE_SPEEX ${ENABLE_EXPERIMENTAL})

add_feature_info (BUILD_SHARED_LIBS BUILD_SHARED_LIBS "build shared libraries")
add_feature_info (ENABLE_EXTERNAL_LIBS ENABLE_EXTERNAL_LIBS "enable FLAC, Vorbis, and Opus codecs")
add_feature_info (ENABLE_MPEG ENABLE_MPEG "enable MPEG audio (including mp3) codecs")
add_feature_info (ENABLE_EXPERIMENTAL ENABLE_EXPERIMENTAL "enable experimental code")
add_feature_info (BUILD_TESTING BUILD_TESTING "build tests")
add_feature_info (BUILD_REGTEST BUILD_REGTEST "build regtest")
add_feature_info (ENABLE_CPACK ENABLE_CPACK "enable CPack support")
add_feature_info (ENABLE_BOW_DOCS ENABLE_BOW_DOCS "enable black-on-white html docs")
add_feature_info (ENABLE_PACKAGE_CONFIG ENABLE_PACKAGE_CONFIG "generate and install package config file")
add_feature_info (INSTALL_PKGCONFIG_MODULE INSTALL_PKGCONFIG_MODULE "generate and install pkg-config module")
add_feature_info (INSTALL_MANPAGES INSTALL_MANPAGES "install man pages for programs")
if (WIN32 AND BUILD_SHARED_LIBS)
	add_feature_info (ENABLE_COMPATIBLE_LIBSNDFILE_NAME ENABLE_COMPATIBLE_LIBSNDFILE_NAME "Set DLL name to libsndfile-1.dll (canonical name), sndfile.dll otherwise")
endif ()

if (HAVE_MSSE2_COMPILER_FLAG)
	add_feature_info (ENABLE_SSE2 ENABLE_SSE2 "add SSE2 compiler flag")
endif ()




set_package_properties (Ogg PROPERTIES
	TYPE RECOMMENDED
	URL "www.xiph.org/ogg/"
	DESCRIPTION "library for manipulating ogg bitstreams"
	PURPOSE "Required to enable Vorbis, Speex, and Opus support"
	)
set_package_properties (Vorbis PROPERTIES
	TYPE RECOMMENDED
	URL "www.vorbis.com/"
	DESCRIPTION "open source lossy audio codec"
	PURPOSE "Enables Vorbis support"
	)
set_package_properties (FLAC PROPERTIES
	TYPE RECOMMENDED
	URL "www.xiph.org/flac/"
	DESCRIPTION "Free Lossless Audio Codec Library"
	PURPOSE "Enables FLAC support"
	)
set_package_properties (mp3lame PROPERTIES
	TYPE RECOMMENDED
	URL "https://lame.sourceforge.io/"
	DESCRIPTION "High quality MPEG Audio Layer III (MP3) encoder"
	PURPOSE "Enables MPEG layer III (MP3) writing support"
	)
set_package_properties (mpg123 PROPERTIES
	TYPE RECOMMENDED
	URL "https://www.mpg123.de/"
	DESCRIPTION "MPEG Audio Layer I/II/III decoder"
	PURPOSE "Enables MPEG Audio reading support"
	)
set_package_properties(Opus PROPERTIES
	TYPE RECOMMENDED
	URL	"www.opus-codec.org/"
	DESCRIPTION "Standardized open source low-latency fullband codec"
	PURPOSE	"Enables experimental Opus support"
	)
set_package_properties(Speex PROPERTIES TYPE OPTIONAL
	URL "www.speex.org/"
	DESCRIPTION "an audio codec tuned for speech"
	PURPOSE "Enables experimental Speex support"
	)
set_package_properties (SQLite3 PROPERTIES
	TYPE OPTIONAL
	URL "www.sqlite.org/"
	DESCRIPTION "light weight SQL database engine."
	PURPOSE "Enables regtest"
	)
if (BUILD_SHARED_LIBS)
	set_package_properties (PythonInterp PROPERTIES
		TYPE REQUIRED
		URL "www.python.org/"
		DESCRIPTION "Python is a widely used high-level programming language."
		PURPOSE "Required to build shared libraries"
		)
endif()

feature_summary (WHAT ALL)

#
# Setup configuration
#

configure_file (src/config.h.cmake src/config.h)

#
# libsndfile
#

# Public libsndfile headers
set (sndfile_HDRS
	include/sndfile.h
	include/sndfile.hh
	)

#
# libsndfile static library
#

add_library (sndfile
	src/sfconfig.h
	src/sfendian.h
	src/sf_unistd.h
	src/common.h
	src/common.c
	src/file_io.c
	src/command.c
	src/pcm.c
	src/ulaw.c
	src/alaw.c
	src/float32.c
	src/double64.c
	src/ima_adpcm.c
	src/ms_adpcm.c
	src/gsm610.c
	src/dwvw.c
	src/vox_adpcm.c
	src/interleave.c
	src/strings.c
	src/dither.c
	src/cart.c
	src/broadcast.c
	src/audio_detect.c
 	src/ima_oki_adpcm.c
	src/ima_oki_adpcm.h
	src/alac.c
	src/chunk.c
	src/ogg.h
	src/ogg.c
	src/chanmap.h
	src/chanmap.c
	src/id3.h
	src/id3.c
	$<$<BOOL:${WIN32}>:src/windows.c>
	src/sndfile.c
	src/aiff.c
	src/au.c
	src/avr.c
	src/caf.c
	src/dwd.c
	src/flac.c
	src/g72x.c
	src/htk.c
	src/ircam.c
	src/macos.c
	src/mat4.c
	src/mat5.c
	src/nist.c
	src/paf.c
	src/pvf.c
	src/raw.c
	src/rx2.c
	src/sd2.c
	src/sds.c
	src/svx.c
	src/txw.c
	src/voc.c
	src/wve.c
	src/w64.c
	src/wavlike.h
	src/wavlike.c
	src/wav.c
	src/xi.c
	src/mpc2k.c
	src/rf64.c
	src/ogg_vorbis.c
	src/ogg_speex.c
	src/ogg_pcm.c
	src/ogg_opus.c
	src/ogg_vcomment.h
	src/ogg_vcomment.c
	src/nms_adpcm.c
	src/mpeg.c
	src/mpeg_decode.c
	src/mpeg_l3_encode.c
	src/GSM610/config.h
	src/GSM610/gsm.h
	src/GSM610/gsm610_priv.h
	src/GSM610/add.c
	src/GSM610/code.c
	src/GSM610/decode.c
	src/GSM610/gsm_create.c
	src/GSM610/gsm_decode.c
	src/GSM610/gsm_destroy.c
	src/GSM610/gsm_encode.c
	src/GSM610/gsm_option.c
	src/GSM610/long_term.c
	src/GSM610/lpc.c
	src/GSM610/preprocess.c
	src/GSM610/rpe.c
	src/GSM610/short_term.c
	src/GSM610/table.c
	src/G72x/g72x.h
	src/G72x/g72x_priv.h
	src/G72x/g721.c
	src/G72x/g723_16.c
	src/G72x/g723_24.c
	src/G72x/g723_40.c
	src/G72x/g72x.c
	src/ALAC/ALACAudioTypes.h
	src/ALAC/ALACBitUtilities.h
	src/ALAC/EndianPortable.h
	src/ALAC/aglib.h
	src/ALAC/dplib.h
	src/ALAC/matrixlib.h
	src/ALAC/alac_codec.h
	src/ALAC/shift.h
	src/ALAC/ALACBitUtilities.c
	src/ALAC/ag_dec.c
	src/ALAC/ag_enc.c
	src/ALAC/dp_dec.c
	src/ALAC/dp_enc.c
	src/ALAC/matrix_dec.c
	src/ALAC/matrix_enc.c
	src/ALAC/alac_decoder.c
	src/ALAC/alac_encoder.c
	${sndfile_HDRS}
	${CMAKE_CURRENT_BINARY_DIR}/src/config.h
	)

add_library (SndFile::sndfile ALIAS sndfile)

target_include_directories (sndfile
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	PRIVATE
		src
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
	)
target_link_libraries (sndfile
	PRIVATE
		$<$<BOOL:${LIBM_REQUIRED}>:m>
		$<$<BOOL:${HAVE_EXTERNAL_XIPH_LIBS}>:Ogg::ogg>
		$<$<BOOL:${HAVE_EXTERNAL_XIPH_LIBS}>:Vorbis::vorbisenc>
		$<$<BOOL:${HAVE_EXTERNAL_XIPH_LIBS}>:FLAC::FLAC>
		$<$<AND:$<BOOL:${ENABLE_EXPERIMENTAL}>,$<BOOL:${HAVE_EXTERNAL_XIPH_LIBS}>,$<BOOL:${HAVE_SPEEX}>>:Speex::Speex>
		$<$<BOOL:${HAVE_EXTERNAL_XIPH_LIBS}>:Opus::opus>
		$<$<BOOL:${HAVE_MPEG}>:MPG123::libmpg123>
		$<$<BOOL:${HAVE_MPEG}>:mp3lame::mp3lame>
	)
set_target_properties (sndfile PROPERTIES
	PUBLIC_HEADER "${sndfile_HDRS}"
	)

if (ENABLE_COMPATIBLE_LIBSNDFILE_NAME)
	if (MINGW OR CYGWIN)
		set_target_properties (sndfile PROPERTIES
			RUNTIME_OUTPUT_NAME "sndfile-1"
			)
	else ()
		set_target_properties (sndfile PROPERTIES
			RUNTIME_OUTPUT_NAME "libsndfile-1"
			)
	endif ()
endif ()

if (BUILD_SHARED_LIBS)

	#
	# ABI version of library.
	#

	#
	# Read libtool version from `configure.ac` and set libsndfile ABI version:
	#
	#   SNDFILE_ABI_VERSION_MAJOR
	#   SNDFILE_ABI_VERSION_MINOR
	#   SNDFILE_ABI_VERSION_PATCH
	#   SNDFILE_ABI_VERSION
	#
	# and Mach-O current and compatibility versions:
	#
	#   SNDFILE_MACHO_CURRENT_VERSION
	#   SNDFILE_MACHO_COMPATIBILITY_VERSION
	#

	include (SetupABIVersions)

	setup_abi_versions()

	if (WIN32)
		set (VERSION_MAJOR ${CPACK_PACKAGE_VERSION_MAJOR})
		set (GEN_TOOL cmake)
		
		set (WIN_RC_VERSION "${CPACK_PACKAGE_VERSION_MAJOR},${CPACK_PACKAGE_VERSION_MINOR},${CPACK_PACKAGE_VERSION_PATCH}")
		set (CLEAN_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
		set (PACKAGE_VERSION ${CPACK_PACKAGE_VERSION})

		configure_file (src/version-metadata.rc.in src/version-metadata.rc @ONLY)
		target_sources (sndfile PRIVATE ${PROJECT_BINARY_DIR}/src/version-metadata.rc)
	endif ()


	set_target_properties (sndfile PROPERTIES
		SOVERSION ${SNDFILE_ABI_VERSION_MAJOR}
		VERSION ${SNDFILE_ABI_VERSION}
		)

	if (APPLE)
		if (NOT (CMAKE_VERSION VERSION_LESS 3.17))
			set_target_properties (sndfile PROPERTIES
				MACHO_CURRENT_VERSION ${SNDFILE_MACHO_CURRENT_VERSION}
				MACHO_COMPATIBILITY_VERSION ${SNDFILE_MACHO_COMPATIBILITY_VERSION}
				)
		else ()
			message (FATAL_ERROR "Apple platform requires cmake >= 3.17 to build dylib.")
		endif ()
	endif ()

	# Symbol files generation

	if (WIN32)
		set (SYMBOL_FILENAME "sndfile.def")
		set (SYMBOL_OS "win32")
	elseif ((CMAKE_SYSTEM_NAME MATCHES "Darwin") OR (CMAKE_SYSTEM_NAME MATCHES "Rhapsody"))
		set (SYMBOL_FILENAME "Symbols.darwin")
		set (SYMBOL_OS "darwin")
	elseif (CMAKE_SYSTEM_NAME MATCHES "OS2")
		set (SYMBOL_FILENAME "Symbols.os2")
		set (SYMBOL_OS "os2")
	elseif (UNIX)
		set (SYMBOL_FILENAME "Symbols.gnu-binutils")
		set (SYMBOL_OS "linux")
	endif ()

	if (DEFINED SYMBOL_OS)
		add_custom_command (
			OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME}
			COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/src/create_symbols_file.py ${SYMBOL_OS} ${SNDFILE_ABI_VERSION} > ${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME}
			COMMENT "Generating ${SYMBOL_FILENAME}..."
			)

		add_custom_target (GENFILES DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME})
		if (SYMBOL_OS MATCHES "win32")
			target_sources (sndfile
				PRIVATE
					${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME}
				)
		elseif (SYMBOL_OS MATCHES "darwin")
			add_dependencies (sndfile GENFILES)
			if (CMAKE_VERSION VERSION_LESS 3.13)
				set_property (TARGET sndfile APPEND_STRING PROPERTY
					LINK_FLAGS "-Wl,-exported_symbols_list -Wl,${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME}"
					)
			else ()
				target_link_options (sndfile PRIVATE "LINKER:-exported_symbols_list,${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME}")
			endif()
		elseif (SYMBOL_OS MATCHES "os")
			add_dependencies (sndfile GENFILES)
			if (CMAKE_VERSION VERSION_LESS 3.13)
				set_property (TARGET sndfile APPEND_STRING PROPERTY
					LINK_FLAGS "-Wl,-export-symbols ${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME}"
					)
			else ()
				target_link_options (sndfile PRIVATE "LINKER:-export-symbols ${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME}")
			endif()
		elseif (UNIX)
			add_dependencies (sndfile GENFILES)
			if (CMAKE_VERSION VERSION_LESS 3.13)
				set_property (TARGET sndfile APPEND_STRING PROPERTY
					LINK_FLAGS "-Wl,--version-script,${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME}"
					)
			else ()
				target_link_options (sndfile PRIVATE "LINKER:--version-script,${CMAKE_CURRENT_BINARY_DIR}/src/${SYMBOL_FILENAME}")
			endif()
		endif()
	endif()

endif ()

#
# Installation
#
install (TARGETS sndfile ${sdnfile_PROGRAMS}
	RUNTIME DESTINATION			${CMAKE_INSTALL_BINDIR}
	ARCHIVE DESTINATION			${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION			${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION	${CMAKE_INSTALL_INCLUDEDIR})

