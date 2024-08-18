/**
 *  ei2081 osx build testing
 *  A macro for debug related printouts which can be enabled at compile time.
 */

#ifndef OSX_DEBUG_H
#define OSX_DEBUG_H

    
    #ifndef DEBUG
    #define DEBUG 0
    #endif /* DEBUG */

    /* there's a way to switch this flag on at compile time, compiler dependant and i forget so HARDCODING IT ON for now
    * COMMENT or REMOVE it or reset it to off if you don't want this prints
    */
    // #define DEBUG 1

    /*
    * this is the macros for debug prints which would exist or not in output executable dependant on above  DEBUG switch.
    *
    * there is a great convo here https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
    */

    /* a generic print statement, use debug_prinf("i say %s", "hi there"); */
    #define DBG_PRINT(fmt, ...) \
                do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

    /* a generic no-params print statement, use debug_prinf("this is just a message for debug"); */
    #define DBG_PRN_MSG(...) \
                do { if (DEBUG) fprintf(stderr, "*** >%s\n", __VA_ARGS__); } while (0)

    /* a block entry print statement, use debug_prinf("whatever block starts"); */
    #define DBG_PRN_NTR(...) \
                do { if (DEBUG) fprintf(stderr, "*** %s...\n", __VA_ARGS__); } while (0)

    /* a block exit print statement, use debug_prinf("whatever block ends"); */
    #define DBG_PRN_XIT(...) \
                do { if (DEBUG) fprintf(stderr, "*** .../%s\n", __VA_ARGS__); } while (0)


#endif /* OSX_DEBUG_H */
