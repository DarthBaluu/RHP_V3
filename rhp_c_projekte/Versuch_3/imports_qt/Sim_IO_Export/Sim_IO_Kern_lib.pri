LIBS += \
        -L$${IMP_DIR}/Sim_IO_Export/$${BUILD_DEBUG_RELEASE}/ -lSim_IO_Kern_lib

PRE_TARGETDEPS += \
        $${IMP_DIR}/Sim_IO_Export/$${BUILD_DEBUG_RELEASE}/libSim_IO_Kern_lib.$${LIB_FILE_EXT}

INCLUDEPATH += \
        $${IMP_DIR}/Sim_IO_Export

DEPENDPATH += \
        $${IMP_DIR}/Sim_IO_Export
