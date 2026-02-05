/**
 * FSIpredefines.h - pcannonProjectStandards
 * Predefines for C and C++ projects
 * STD Information: 20250723 - 1.0S
 */

#ifndef INCLUDE_FSI_FSIPREDEFINES_HPP_
#define INCLUDE_FSI_FSIPREDEFINES_HPP_

// Project setup
#define FSI_DEFAULT_CPP_STD			201703L

// Versioning
#define FSI_VERSION_MAJOR            0
#define FSI_VERSION_MINOR            1
#define FSI_VERSION_PATCH            0

#define FSI_VERSION_STD              20260205

// Version states:
// * dev
// * beta
// * build
#define FSI_VERSION_STATE          "beta"

#define FSI_VERSION                ((FSI_VERSION_MAJOR<<16)|(FSI_VERSION_MINOR<<8)|(FSI_VERSION_PATCH)|(FSI_VERSION_STATE << 24))

#define FSI_VERSION_CHECK(FSI_VERSION_MAJOR, FSI_VERSION_MINOR, FSI_VERSION_PATCH, FSI_VERSION_STATE) \
    (((FSI_VERSION_MAJOR)<<16)|((FSI_VERSION_MINOR)<<8)|(FSI_VERSION_PATCH)|((FSI_VERSION_STATE) << 24))

// Macro utils
#define FSI_STRINGIFY(x) #x
#define FSI_TOSTRING(x) FSI_STRINGIFY(x)

#ifndef FSI_DEV
#   define FSI_DEV true
#endif

#ifdef WIN32
#	define FSI_OS_WIN32
#elif defined(__APPLE__) || defined(__MACH__) || defined(Macintosh)
#	define FSI_OS_MACOS
#elif defined(__linux__) || defined(__unix) || defined(__unix__)
#	define FSI_OS_UNIX_LINUX
#elif defined(__FreeBSD__)
#	define FSI_OS_FREEBSD
#else
#	error "Current platform is not supported"
#endif // defined(WIN32) // Platform check

#endif  // INCLUDE_FSI_FSIPREDEFINES_HPP_

