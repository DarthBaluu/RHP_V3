LIBS += \
        -L$${IMP_DIR}/OS_Support_Export/$${BUILD_DEBUG_RELEASE}/ -lOS_Support_lib

PRE_TARGETDEPS += \
        $${IMP_DIR}/OS_Support_Export/$${BUILD_DEBUG_RELEASE}/libOS_Support_lib.$${LIB_FILE_EXT}

INCLUDEPATH += \
        $${IMP_DIR}/OS_Support_Export

DEPENDPATH += \
        $${IMP_DIR}/OS_Support_Export
