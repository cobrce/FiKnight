#ifndef DEFAULT_CONFIG_H_
#define DEFAULT_CONFIG_H_

// #define USE_PRODUCTION_CONFIG // uncomment this line to disable debug functions (serial reader and commands filter are kept)


// General configuration
#define INFINITE_MAIN_LOOP
#ifndef MAIN_LOOP_DELAY
#define MAIN_LOOP_DELAY 100
#endif

#ifdef USE_PRODUCTION_CONFIG 
// production configuration
    #undef NOTIFY_ON_STATE_CHANGE
    #undef NOTIFY_EXECUTION_STATE
    #define FILTER_DEBUG_COMMANDS
    #undef EXECUTE_DEBUG_COMMANDS
    #undef INCLUDE_DEBUG_FUNCTION
#else
// debug configuration
    #define NOTIFY_ON_STATE_CHANGE
    #define NOTIFY_EXECUTION_STATE

    #ifndef NOTIFICATION_INTERVAL
        #define NOTIFICATION_INTERVAL 2000
    #endif
    #define FILTER_DEBUG_COMMANDS
    #define EXECUTE_DEBUG_COMMANDS
    #define INCLUDE_DEBUG_FUNCTION
#endif
#endif