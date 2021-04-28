
#pragma once
/*
this is bottom line everything uses this 'thing'
thus no dependancies beside crt and win32
*/

#ifdef __clang__
#pragma clang system_header
#endif

#include "ccommon.h"
#include "win32/win32_console.h" // win_enable_vt_100_and_unicode

// -----------------------------------------------------------------------------
DBJ_EXTERN_C_BEGIN

/*
	terror == terminating error
	NOTE: all the bets are of so no point of using some logging
	*/
inline void dbj_terror(const char *msg_, const char *file_, const int line_)
{
	/// DBJ_ASSERT(msg_ && file_ && line_);
	/// all the bets are of so no point of using some logging
	perror("\n\n" DBJ_ERR_PROMPT("\n\ndbj  Terminating error!"));
	exit(EXIT_FAILURE);
}

/*
2020 SEP 03 -- it turns out (again) we need to initialize WIN10 terminal
to show us VT100 colours
*/
inline bool dbj_win_vt100_initor_()
{
	// this can fail for various reasons
	// key reason being we are in the app with no console
	// TODO: do it on different thread
	bool rezult = win_enable_vt_100_and_unicode();

	if (!rezult)
	{
		dbj_terror("win_enable_vt_100_and_unicode() failed!", __FILE__, __LINE__);
	}
	return rezult;
};

	// DBJ: TODO: must think about this
	// basically this
	// should print only in debug builds
#ifdef _DEBUG
#define dbj_release_mode_build (1 == 0)
#define dbj_stderr_print(...)          \
	do                                 \
	{                                  \
		printf(stderr, __VA_ARGS__); \
	} while (0)
#else
#define dbj_release_mode_build (1 == 1)
#define dbj_stderr_print(format_string, ...) (void)format_string
#endif

// naming is hard ... DBJ_PRINT is a wrong name here
// DBG_PRINT  would indicate there is NO release print from here
// -----------------------------------------------------------------------------
#undef DBJ_PRINT
#define DBJ_PRINT(...) dbj_stderr_print(__VA_ARGS__)

#undef DBJ_CHK
#define DBJ_CHK(x)    \
	if (false == (x)) \
	DBJ_PRINT("Evaluated to false! ", DBJ_FLT_PROMPT(x))

///	-----------------------------------------------------------------------------------------
// CAUTION! DBJ_VERIFY works in release builds too
#undef DBJ_VERIFY
#undef DBJ_VERIFY_

#define DBJ_VERIFY_(x, file, line) \
	if (false == x)                \
	dbj_terror("Expression: " #x ", failed ", file, line)

#define DBJ_VERIFY(x) DBJ_VERIFY_(x, __FILE__, __LINE__)

#undef DBJ_PERROR
#ifndef NDEBUG
#define DBJ_PERROR (perror(__FILE__ " # " _CRT_STRINGIZE(__LINE__)))
#else
#define DBJ_PERROR
#endif // NDEBUG

#undef DBJ_FERROR
#ifdef _DEBUG
#define DBJ_FERROR(FP_)       \
	do                        \
	{                         \
		if (ferror(FP_) != 0) \
		{                     \
			DBJ_PERROR;       \
			clearerr_s(FP_);  \
		}                     \
	} while (0)
#else
#define DBJ_FERROR(FP_)
#endif // _DEBUG

#undef DBJ_FAST_FAIL
#ifndef NDEBUG
#define DBJ_FAST_FAIL       \
	do                      \
	{                       \
		DBJ_PERROR;         \
		exit(EXIT_FAILURE); \
		__debugbreak();     \
	} while (0)
#else // !NDEBUG
#define DBJ_FAST_FAIL       \
	do                      \
	{                       \
		DBJ_PERROR;         \
		exit(EXIT_FAILURE); \
	} while (0)
#endif // ! NDEBUG

// some folks like this some like assert philosophy,
// meaning relaxed controls for release builds
// #define VERIFY(X) do { if (!(X)) { perror("\nfatal app error at: "__FILE__ ",\nfailed expression : " #X "\nsystem error "); exit(0); } } while(0)

// NOTE: F must be a string literal
#define B(X_) (X_) ? "true" : "false"
#define P(F, X_) DBJ_PRINT("\n%16s : %4d : %16s :\t" F, __FILE__, __LINE__, #X_, (X_))

// pointer pretty print
// using the advice as per https://stackoverflow.com/a/9053835/10870835
// outcome: same as using '%p` but in all cappitals
// https://godbolt.org/z/eqTsYneP6
#define PP(X_)                                      \
	DBJ_PRINT("\n%16s : %4d : %8s :\t 0x%" PRIXPTR, \
			  __FILE__, __LINE__, #X_, (uintptr_t)(X_))

	// in this version F_ does not have to be string literal
#undef SX
// Show eXpression
#define SX(F_, x_)                                                    \
	do                                                                \
	{                                                                 \
		DBJ_PRINT("\n%4d | %s | %16s : ", __LINE__, __FILE__, (#x_)); \
		DBJ_PRINT(F_, (x_));                                          \
	} while (0)

// boolean expression to string
#undef TF
#define TF(a) (a ? "true" : "false")

// make it a run-time affair
inline void dbj_print_bool_result(const int line, const char *file, const char *expression, const bool result)
{
	DBJ_PRINT("\n%4d | %s | %16s : %s", line, file, expression, (result ? "true" : "false"));
}

#undef PRINB
#define PRINB(x) dbj_print_bool_result(__LINE__, __FILE__, (#x), (x))

DBJ_EXTERN_C_END
