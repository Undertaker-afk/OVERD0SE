#pragma once

#include "..\security\xorstr.h"

/*
 * current build of cheat, change this when you made noticeable changes
 * - used for automatic adaptation mechanism of configuration files from previous versions
 */

#define CS_VERSION 2000
#pragma section(".text")
__declspec(allocate(".text")) const unsigned char _fake_addr[] = { 0xFF, 0x23 };

 /*
 * game's modules
 */
#define CLIENT_DLL L"client.dll"
#define SERVER_DLL L"server.dll"
#define ENGINE2_DLL L"engine2.dll"
#define SCHEMASYSTEM_DLL L"schemasystem.dll"
#define INPUTSYSTEM_DLL L"inputsystem.dll"
#define SDL3_DLL L"SDL3.dll"
#define TIER0_DLL L"tier0.dll"
#define ENGINE2_DLL L"engine2.dll"

#define NAVSYSTEM_DLL L"navsystem.dll"
#define RENDERSYSTEM_DLL L"rendersystemdx11.dll"
#define LOCALIZE_DLL L"localize.dll"
#define DBGHELP_DLL L"dbghelp.dll"
#define GAMEOVERLAYRENDERER_DLL L"GameOverlayRenderer64.dll"
#define PARTICLES_DLL L"particles.dll"
#define SCENESYSTEM_DLL L"scenesystem.dll"
#define MATERIAL_SYSTEM2_DLL L"materialsystem2.dll"
#define MATCHMAKING_DLL L"matchmaking.dll"
#define RESOURCESYSTEM_DLL L"resourcesystem.dll"

//#define CS_XOR(STRING) CXOR(STRING)

#ifdef _DEBUG
#define CS_LOG_CONSOLE
#endif

#define CS_PARANOID
#define CS_PARANOID_PATTERN_UNIQUENESS
#define CS_CONFIGURATION_BINARY

#if defined(i386) || defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(__i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__) || defined(__INTEL__)
#define CS_ARCH_X86
#elif defined(__LP64__) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(_M_X64) || defined(__ia64) || defined(_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
#define CS_ARCH_X64
#else
static_assert(false, "could not determine the target architecture, consider define it manually!");
#endif

#ifdef _MSC_VER
#define CS_COMPILER_MSC
#endif
#ifdef __clang__ // @note: clang-cl have defined both 'CS_COMPILER_CLANG' and 'CS_COMPILER_MSC'
#define CS_COMPILER_CLANG
#endif

#ifdef __has_builtin
#define CS_HAS_BUILTIN(BUILTIN) __has_builtin(BUILTIN)
#else
#define CS_HAS_BUILTIN(BUILTIN) 0
#endif

#ifdef CS_COMPILER_MSC
// treat "discarding return value of function with 'nodiscard' attribute" warning as error
#pragma warning(error: 4834)
#endif

#ifdef CS_COMPILER_CLANG
#pragma clang diagnostic ignored "-Wunused-private-field"
#endif

#if defined(CS_COMPILER_MSC) || defined(CS_COMPILER_CLANG)
#define CS_NAKED __declspec(naked)
#endif

// @todo: platform dependent / but currently we shouldn't give fuck on it
#define CS_CDECL __cdecl
#define CS_STDCALL __stdcall
#define CS_FASTCALL __fastcall
#define CS_THISCALL __thiscall
#define CS_VECTORCALL __vectorcall

#define _CS_INTERNAL_CONCATENATE(LEFT, RIGHT) LEFT##RIGHT
#define _CS_INTERNAL_STRINGIFY(NAME) #NAME
#define _CS_INTERNAL_UNPARENTHESIZE(...) __VA_ARGS__

// convert plain text to string
#define CS_STRINGIFY(NAME) _CS_INTERNAL_STRINGIFY(NAME)
// concatenate plain text
#define CS_CONCATENATE(LEFT, RIGHT) _CS_INTERNAL_CONCATENATE(LEFT, RIGHT)
// unparenthesize variadic arguments
#define CS_UNPARENTHESIZE(...) _CS_INTERNAL_UNPARENTHESIZE(__VA_ARGS__)

// calculate elements count of fixed-size C array
#define CS_ARRAYSIZE(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

// calculate the offset of a struct member variable, in bytes
#if defined(_CRT_USE_BUILTIN_OFFSETOF) || CS_HAS_BUILTIN(__builtin_offsetof)
#define CS_OFFSETOF(STRUCT, MEMBER) __builtin_offsetof(STRUCT, MEMBER)
#else
#define CS_OFFSETOF(STRUCT, MEMBER) reinterpret_cast<std::size_t>(std::addressof(static_cast<STRUCT*>(nullptr)->MEMBER))
#endif

#ifndef CS_NO_RTTI
#if defined(CS_COMPILER_MSC) && !defined(_CPPRTTI)
#define CS_NO_RTTI
#elif defined(CS_COMPILER_CLANG)
#if !__has_feature(cxx_rtti)
#define CS_NO_RTTI
#endif
#endif
#endif

#ifndef CS_INLINE
#if defined(CS_COMPILER_MSC)
#define CS_INLINE __forceinline
#else
// referenced to clang documentation, this is enough: https://clang.llvm.org/compatibility.html
#define CS_INLINE inline
#endif
#endif

#ifndef CS_RETURN_ADDRESS
#if defined(CS_COMPILER_MSC)
#define CS_RETURN_ADDRESS() _ReturnAddress()
#elif defined(CS_COMPILER_CLANG)
#define CS_RETURN_ADDRESS() __builtin_return_address(0)
#else
static_assert(false, "it is expected you to define CS_RETURN_ADDRESS() into something that will get the return address off the stack!")
#define CS_RETURN_ADDRESS()
#endif
#endif

#ifndef CS_FRAME_ADDRESS
#if defined(CS_COMPILER_MSC)
#define CS_FRAME_ADDRESS() _AddressOfReturnAddress()
#elif defined(CS_COMPILER_CLANG)
// @note: it isn't always what we're expecting, compiler dependent
#define CS_FRAME_ADDRESS() __builtin_frame_address(0)
#else
static_assert(false, "it is expected you to define CS_FRAME_ADDRESS() into something that will get the address of the method's stack frame!")
#define CS_FRAME_ADDRESS()
#endif
#endif

#ifndef CS_DEBUG_BREAK
#if defined(CS_COMPILER_MSC)
#define CS_DEBUG_BREAK() __debugbreak()
#elif defined(CS_COMPILER_CLANG)
#define CS_DEBUG_BREAK() __builtin_debugtrap()
#else
static_assert(false, "it is expected you to define CS_DEBUG_BREAK() into something that will break in a debugger!");
#define CS_DEBUG_BREAK()
#endif
#endif

#ifndef CS_ASSERT
#ifdef _DEBUG
#define CS_ASSERT(EXPRESSION) static_cast<void>(!!(EXPRESSION) || (CS_DEBUG_BREAK(), 0))
#else
// disable assertion for release builds
#define CS_ASSERT(EXPRESSION) static_cast<void>(!!(EXPRESSION) || (CS_DEBUG_BREAK(), 0))
#endif
#endif

#if !defined(CS_CONFIGURATION_BINARY) && !defined(CS_CONFIGURATION_JSON) && !defined(CS_CONFIGURATION_TOML)
static_assert(false, "it is expected you to define one of the available configuration file formatters!");

// fallback to binary formatter by default
#define CS_CONFIGURATION_BINARY
#endif

/*
 * explicitly delete the following constructors, to prevent attempts on using them:
 * constructor, move-constructor, copy-constructor
 */
#define CS_CLASS_NO_CONSTRUCTOR(CLASS)	\
CLASS() = delete;						\
CLASS(CLASS&&) = delete;				\
CLASS(const CLASS&) = delete;

 /*
  * explicitly delete the following assignment operators, to prevent attempts on using them:
  * move-assignment, copy-assignment
  */
#define CS_CLASS_NO_ASSIGNMENT(CLASS)	\
CLASS& operator=(CLASS&&) = delete;		\
CLASS& operator=(const CLASS&) = delete;

  // explicitly delete any class initializer to prevent attempts on using them
#define CS_CLASS_NO_INITIALIZER(CLASS)	\
CS_CLASS_NO_CONSTRUCTOR(CLASS)			\
CS_CLASS_NO_ASSIGNMENT(CLASS)

// explicitly delete class heap allocator and deallocator, to prevent attempts on using class at heap memory
#define CS_CLASS_NO_ALLOC()								\
void* operator new(const std::size_t nSize) = delete;	\
void operator delete(void* pMemory) = delete;