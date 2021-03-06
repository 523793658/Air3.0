if(WIN32)
    if(MSVC AND (CMAKE_GENERATOR MATCHES "Visual Studio"))
        if((CMAKE_GENERATOR_PLATFORM STREQUAL "x64") OR (CMAKE_GENERATOR MATCHES "Win64"))
            SET(AIR_ARCH_NAME "x64")
            SET(AIR_VS_PLATFORM_NAME "x64")
        elseif((CMAKE_GENERATOR_PLATFORM STREQUAL "ARM64") OR (CMAKE_GENERATOR MATCHES "ARM64"))
            SET(AIR_ARCH_NAME "arm64")
            SET(AIR_VS_PLATFORM_NAME "ARM64")
        elseif((CMAKE_GENERATOR_PLATFORM STREQUAL "ARM") OR (CMAKE_GENERATOR MATCHES "ARM"))
            SET(AIR_ARCH_NAME "arm")
            SET(AIR_VS_PLATFORM_NAME "ARM")
        else()
            MESSAGE(FATAL_ERROR "This CPU architecture is not supported")
        endif()
    endif()
    SET(AIR_PLATFORM_NAME "win")
    SET(AIR_PLATFORM_WINDOWS_DESKTOP TRUE)
    set(AIR_PLATFORM_WINDOWS TRUE)
elseif(ANDROID)
    set(AIR_PLATFORM_NAME "android")
    set(AIR_PLATFORM_ANDROID TRUE)
endif()



if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(AIR_HOST_PLATFORM_NAME "win")
    set(AIR_HOST_PLATFORM_WINDOWS TRUE)
endif()

if(NOT AIR_ARCH_NAME)
    if((CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64") OR (CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64"))
        set(AIR_ARCH_NAME "x64")
    else()
        set(AIR_ARCH_NAME "x86")
    endif()
endif()

if((CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "AMD64") OR (CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64"))
    SET(AIR_HOST_ARCH_NAME "x64")
else()
    SET(AIR_HOST_ARCH_NAME "x86")
endif()


SET(AIR_PLATFORM_NAME ${AIR_PLATFORM_NAME}_${AIR_ARCH_NAME})


SET(AIR_HOST_PLATFORM_NAME ${AIR_HOST_PLATFORM_NAME}_${AIR_HOST_ARCH_NAME})

SET(AIR_PREFERRED_LIB_TYPE "SHARED")

if(AIR_PLATFORM_WINDOWS_DESKTOP)
    set(AIR_IS_DEV_PLATFORM TRUE)
else()
    set(AIR_IS_DEV_PLATFORM FALSE)
endif()

if(AIR_PLATFORM_WINDOWS)
    add_compile_definitions(PLATFORM_WINDOWS)
endif()