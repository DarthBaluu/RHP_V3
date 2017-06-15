LIBS += \
        -L$${IMP_DIR}/Free_RTOS_Export/$${BUILD_DEBUG_RELEASE}/ -lFree_RTOS_Kern_lib

PRE_TARGETDEPS += \
        $${IMP_DIR}/Free_RTOS_Export/$${BUILD_DEBUG_RELEASE}/libFree_RTOS_Kern_lib.$${LIB_FILE_EXT}

INCLUDEPATH += \
        $${IMP_DIR}/Free_RTOS_Export \
        $${IMP_DIR}/Free_RTOS_Export/FreeRTOS/include

DEPENDPATH += \
        $${IMP_DIR}/Free_RTOS_Export \
        $${IMP_DIR}/Free_RTOS_Export/FreeRTOS/include
