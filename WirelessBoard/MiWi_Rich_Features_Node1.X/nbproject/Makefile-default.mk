#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MiWi_Rich_Features_Node1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MiWi_Rich_Features_Node1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=DemoOutput.c HardwareProfile.c MAC_EEProm.c ../../../../CodeBank8/bsp/Microchip/delays/delays.c ../../../../CodeBank8/bsp/Microchip/gpios/gpios.c ../../../../CodeBank8/bsp/Microchip/spi/spi.c ../../../../CodeBank8/bsp/Microchip/system/system.c ../../../../microchip_solutions_v2013-06-15/Microchip/Common/LCDBlocking.c ../../../../microchip_solutions_v2013-06-15/Microchip/Common/TimeDelay.c ../../../../CodeBank8/modules/st7032/st7032.c ../../../../CodeBank8/modules/eeprom25xx/eeprom25xx.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWi/MiWi.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWiPRO/MiWiPRO.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/P2P/P2P.c SW_I2C/swacki2c.c SW_I2C/swckti2c.c SW_I2C/swgtci2c.c SW_I2C/swgtsi2c.c SW_I2C/swptci2c.c SW_I2C/swptsi2c.c SW_I2C/swrsti2c.c SW_I2C/swstpi2c.c SW_I2C/swstri2c.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/Console.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/EEPROM.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MSPI.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/NVM.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/SymbolTime.c ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/MRF89XA/MRF89XA.c ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/crc.c ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/security.c "Cisternas Unison 2.0.c"

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/DemoOutput.o ${OBJECTDIR}/HardwareProfile.o ${OBJECTDIR}/MAC_EEProm.o ${OBJECTDIR}/_ext/1557261057/delays.o ${OBJECTDIR}/_ext/739875083/gpios.o ${OBJECTDIR}/_ext/1390700963/spi.o ${OBJECTDIR}/_ext/1109127202/system.o ${OBJECTDIR}/_ext/426468601/LCDBlocking.o ${OBJECTDIR}/_ext/426468601/TimeDelay.o ${OBJECTDIR}/_ext/679473599/st7032.o ${OBJECTDIR}/_ext/42883563/eeprom25xx.o ${OBJECTDIR}/_ext/1882031954/MiWi.o ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o ${OBJECTDIR}/_ext/1584730194/P2P.o ${OBJECTDIR}/SW_I2C/swacki2c.o ${OBJECTDIR}/SW_I2C/swckti2c.o ${OBJECTDIR}/SW_I2C/swgtci2c.o ${OBJECTDIR}/SW_I2C/swgtsi2c.o ${OBJECTDIR}/SW_I2C/swptci2c.o ${OBJECTDIR}/SW_I2C/swptsi2c.o ${OBJECTDIR}/SW_I2C/swrsti2c.o ${OBJECTDIR}/SW_I2C/swstpi2c.o ${OBJECTDIR}/SW_I2C/swstri2c.o ${OBJECTDIR}/_ext/554139889/Console.o ${OBJECTDIR}/_ext/554139889/EEPROM.o ${OBJECTDIR}/_ext/554139889/MSPI.o ${OBJECTDIR}/_ext/554139889/NVM.o ${OBJECTDIR}/_ext/554139889/SymbolTime.o ${OBJECTDIR}/_ext/1782009031/MRF89XA.o ${OBJECTDIR}/_ext/293229859/crc.o ${OBJECTDIR}/_ext/293229859/security.o "${OBJECTDIR}/Cisternas Unison 2.0.o"
POSSIBLE_DEPFILES=${OBJECTDIR}/DemoOutput.o.d ${OBJECTDIR}/HardwareProfile.o.d ${OBJECTDIR}/MAC_EEProm.o.d ${OBJECTDIR}/_ext/1557261057/delays.o.d ${OBJECTDIR}/_ext/739875083/gpios.o.d ${OBJECTDIR}/_ext/1390700963/spi.o.d ${OBJECTDIR}/_ext/1109127202/system.o.d ${OBJECTDIR}/_ext/426468601/LCDBlocking.o.d ${OBJECTDIR}/_ext/426468601/TimeDelay.o.d ${OBJECTDIR}/_ext/679473599/st7032.o.d ${OBJECTDIR}/_ext/42883563/eeprom25xx.o.d ${OBJECTDIR}/_ext/1882031954/MiWi.o.d ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o.d ${OBJECTDIR}/_ext/1584730194/P2P.o.d ${OBJECTDIR}/SW_I2C/swacki2c.o.d ${OBJECTDIR}/SW_I2C/swckti2c.o.d ${OBJECTDIR}/SW_I2C/swgtci2c.o.d ${OBJECTDIR}/SW_I2C/swgtsi2c.o.d ${OBJECTDIR}/SW_I2C/swptci2c.o.d ${OBJECTDIR}/SW_I2C/swptsi2c.o.d ${OBJECTDIR}/SW_I2C/swrsti2c.o.d ${OBJECTDIR}/SW_I2C/swstpi2c.o.d ${OBJECTDIR}/SW_I2C/swstri2c.o.d ${OBJECTDIR}/_ext/554139889/Console.o.d ${OBJECTDIR}/_ext/554139889/EEPROM.o.d ${OBJECTDIR}/_ext/554139889/MSPI.o.d ${OBJECTDIR}/_ext/554139889/NVM.o.d ${OBJECTDIR}/_ext/554139889/SymbolTime.o.d ${OBJECTDIR}/_ext/1782009031/MRF89XA.o.d ${OBJECTDIR}/_ext/293229859/crc.o.d ${OBJECTDIR}/_ext/293229859/security.o.d "${OBJECTDIR}/Cisternas Unison 2.0.o.d"

# Object Files
OBJECTFILES=${OBJECTDIR}/DemoOutput.o ${OBJECTDIR}/HardwareProfile.o ${OBJECTDIR}/MAC_EEProm.o ${OBJECTDIR}/_ext/1557261057/delays.o ${OBJECTDIR}/_ext/739875083/gpios.o ${OBJECTDIR}/_ext/1390700963/spi.o ${OBJECTDIR}/_ext/1109127202/system.o ${OBJECTDIR}/_ext/426468601/LCDBlocking.o ${OBJECTDIR}/_ext/426468601/TimeDelay.o ${OBJECTDIR}/_ext/679473599/st7032.o ${OBJECTDIR}/_ext/42883563/eeprom25xx.o ${OBJECTDIR}/_ext/1882031954/MiWi.o ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o ${OBJECTDIR}/_ext/1584730194/P2P.o ${OBJECTDIR}/SW_I2C/swacki2c.o ${OBJECTDIR}/SW_I2C/swckti2c.o ${OBJECTDIR}/SW_I2C/swgtci2c.o ${OBJECTDIR}/SW_I2C/swgtsi2c.o ${OBJECTDIR}/SW_I2C/swptci2c.o ${OBJECTDIR}/SW_I2C/swptsi2c.o ${OBJECTDIR}/SW_I2C/swrsti2c.o ${OBJECTDIR}/SW_I2C/swstpi2c.o ${OBJECTDIR}/SW_I2C/swstri2c.o ${OBJECTDIR}/_ext/554139889/Console.o ${OBJECTDIR}/_ext/554139889/EEPROM.o ${OBJECTDIR}/_ext/554139889/MSPI.o ${OBJECTDIR}/_ext/554139889/NVM.o ${OBJECTDIR}/_ext/554139889/SymbolTime.o ${OBJECTDIR}/_ext/1782009031/MRF89XA.o ${OBJECTDIR}/_ext/293229859/crc.o ${OBJECTDIR}/_ext/293229859/security.o ${OBJECTDIR}/Cisternas\ Unison\ 2.0.o

# Source Files
SOURCEFILES=DemoOutput.c HardwareProfile.c MAC_EEProm.c ../../../../CodeBank8/bsp/Microchip/delays/delays.c ../../../../CodeBank8/bsp/Microchip/gpios/gpios.c ../../../../CodeBank8/bsp/Microchip/spi/spi.c ../../../../CodeBank8/bsp/Microchip/system/system.c ../../../../microchip_solutions_v2013-06-15/Microchip/Common/LCDBlocking.c ../../../../microchip_solutions_v2013-06-15/Microchip/Common/TimeDelay.c ../../../../CodeBank8/modules/st7032/st7032.c ../../../../CodeBank8/modules/eeprom25xx/eeprom25xx.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWi/MiWi.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWiPRO/MiWiPRO.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/P2P/P2P.c SW_I2C/swacki2c.c SW_I2C/swckti2c.c SW_I2C/swgtci2c.c SW_I2C/swgtsi2c.c SW_I2C/swptci2c.c SW_I2C/swptsi2c.c SW_I2C/swrsti2c.c SW_I2C/swstpi2c.c SW_I2C/swstri2c.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/Console.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/EEPROM.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MSPI.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/NVM.c ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/SymbolTime.c ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/MRF89XA/MRF89XA.c ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/crc.c ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/security.c Cisternas Unison 2.0.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/MiWi_Rich_Features_Node1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F47J13
MP_PROCESSOR_OPTION_LD=18f47j13
MP_LINKER_DEBUG_OPTION=  -u_DEBUGSTACK
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/DemoOutput.o: DemoOutput.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DemoOutput.o.d 
	@${RM} ${OBJECTDIR}/DemoOutput.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/DemoOutput.o   DemoOutput.c 
	@${DEP_GEN} -d ${OBJECTDIR}/DemoOutput.o 
	@${FIXDEPS} "${OBJECTDIR}/DemoOutput.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/HardwareProfile.o: HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/HardwareProfile.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/HardwareProfile.o   HardwareProfile.c 
	@${DEP_GEN} -d ${OBJECTDIR}/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/MAC_EEProm.o: MAC_EEProm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MAC_EEProm.o.d 
	@${RM} ${OBJECTDIR}/MAC_EEProm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/MAC_EEProm.o   MAC_EEProm.c 
	@${DEP_GEN} -d ${OBJECTDIR}/MAC_EEProm.o 
	@${FIXDEPS} "${OBJECTDIR}/MAC_EEProm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1557261057/delays.o: ../../../../CodeBank8/bsp/Microchip/delays/delays.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1557261057" 
	@${RM} ${OBJECTDIR}/_ext/1557261057/delays.o.d 
	@${RM} ${OBJECTDIR}/_ext/1557261057/delays.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1557261057/delays.o   ../../../../CodeBank8/bsp/Microchip/delays/delays.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1557261057/delays.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1557261057/delays.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/739875083/gpios.o: ../../../../CodeBank8/bsp/Microchip/gpios/gpios.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/739875083" 
	@${RM} ${OBJECTDIR}/_ext/739875083/gpios.o.d 
	@${RM} ${OBJECTDIR}/_ext/739875083/gpios.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/739875083/gpios.o   ../../../../CodeBank8/bsp/Microchip/gpios/gpios.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/739875083/gpios.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/739875083/gpios.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1390700963/spi.o: ../../../../CodeBank8/bsp/Microchip/spi/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1390700963" 
	@${RM} ${OBJECTDIR}/_ext/1390700963/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1390700963/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1390700963/spi.o   ../../../../CodeBank8/bsp/Microchip/spi/spi.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1390700963/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1390700963/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1109127202/system.o: ../../../../CodeBank8/bsp/Microchip/system/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1109127202" 
	@${RM} ${OBJECTDIR}/_ext/1109127202/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1109127202/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1109127202/system.o   ../../../../CodeBank8/bsp/Microchip/system/system.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1109127202/system.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1109127202/system.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/426468601/LCDBlocking.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Common/LCDBlocking.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/426468601" 
	@${RM} ${OBJECTDIR}/_ext/426468601/LCDBlocking.o.d 
	@${RM} ${OBJECTDIR}/_ext/426468601/LCDBlocking.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/426468601/LCDBlocking.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Common/LCDBlocking.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/426468601/LCDBlocking.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/426468601/LCDBlocking.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/426468601/TimeDelay.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/426468601" 
	@${RM} ${OBJECTDIR}/_ext/426468601/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/426468601/TimeDelay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/426468601/TimeDelay.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Common/TimeDelay.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/426468601/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/426468601/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/679473599/st7032.o: ../../../../CodeBank8/modules/st7032/st7032.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/679473599" 
	@${RM} ${OBJECTDIR}/_ext/679473599/st7032.o.d 
	@${RM} ${OBJECTDIR}/_ext/679473599/st7032.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/679473599/st7032.o   ../../../../CodeBank8/modules/st7032/st7032.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/679473599/st7032.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/679473599/st7032.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/42883563/eeprom25xx.o: ../../../../CodeBank8/modules/eeprom25xx/eeprom25xx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/42883563" 
	@${RM} ${OBJECTDIR}/_ext/42883563/eeprom25xx.o.d 
	@${RM} ${OBJECTDIR}/_ext/42883563/eeprom25xx.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/42883563/eeprom25xx.o   ../../../../CodeBank8/modules/eeprom25xx/eeprom25xx.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/42883563/eeprom25xx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/42883563/eeprom25xx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1882031954/MiWi.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWi/MiWi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1882031954" 
	@${RM} ${OBJECTDIR}/_ext/1882031954/MiWi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1882031954/MiWi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1882031954/MiWi.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWi/MiWi.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1882031954/MiWi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1882031954/MiWi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1110780129/MiWiPRO.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWiPRO/MiWiPRO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1110780129" 
	@${RM} ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o.d 
	@${RM} ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWiPRO/MiWiPRO.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1110780129/MiWiPRO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1584730194/P2P.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/P2P/P2P.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1584730194" 
	@${RM} ${OBJECTDIR}/_ext/1584730194/P2P.o.d 
	@${RM} ${OBJECTDIR}/_ext/1584730194/P2P.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1584730194/P2P.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/P2P/P2P.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1584730194/P2P.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1584730194/P2P.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swacki2c.o: SW_I2C/swacki2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swacki2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swacki2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swacki2c.o   SW_I2C/swacki2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swacki2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swacki2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swckti2c.o: SW_I2C/swckti2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swckti2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swckti2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swckti2c.o   SW_I2C/swckti2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swckti2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swckti2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swgtci2c.o: SW_I2C/swgtci2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swgtci2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swgtci2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swgtci2c.o   SW_I2C/swgtci2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swgtci2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swgtci2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swgtsi2c.o: SW_I2C/swgtsi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swgtsi2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swgtsi2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swgtsi2c.o   SW_I2C/swgtsi2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swgtsi2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swgtsi2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swptci2c.o: SW_I2C/swptci2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swptci2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swptci2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swptci2c.o   SW_I2C/swptci2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swptci2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swptci2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swptsi2c.o: SW_I2C/swptsi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swptsi2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swptsi2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swptsi2c.o   SW_I2C/swptsi2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swptsi2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swptsi2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swrsti2c.o: SW_I2C/swrsti2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swrsti2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swrsti2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swrsti2c.o   SW_I2C/swrsti2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swrsti2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swrsti2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swstpi2c.o: SW_I2C/swstpi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swstpi2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swstpi2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swstpi2c.o   SW_I2C/swstpi2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swstpi2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swstpi2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swstri2c.o: SW_I2C/swstri2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swstri2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swstri2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swstri2c.o   SW_I2C/swstri2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swstri2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swstri2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/Console.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/Console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/Console.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/Console.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/Console.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/Console.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/Console.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/Console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/EEPROM.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/EEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/EEPROM.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/EEPROM.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/EEPROM.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/EEPROM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/MSPI.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MSPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/MSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/MSPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/MSPI.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MSPI.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/MSPI.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/MSPI.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/NVM.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/NVM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/NVM.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/NVM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/NVM.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/NVM.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/NVM.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/NVM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/SymbolTime.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/SymbolTime.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/SymbolTime.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/SymbolTime.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/SymbolTime.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/SymbolTime.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/SymbolTime.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/SymbolTime.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1782009031/MRF89XA.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/MRF89XA/MRF89XA.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1782009031" 
	@${RM} ${OBJECTDIR}/_ext/1782009031/MRF89XA.o.d 
	@${RM} ${OBJECTDIR}/_ext/1782009031/MRF89XA.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1782009031/MRF89XA.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/MRF89XA/MRF89XA.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1782009031/MRF89XA.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1782009031/MRF89XA.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/293229859/crc.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/293229859" 
	@${RM} ${OBJECTDIR}/_ext/293229859/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/293229859/crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/293229859/crc.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/crc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/293229859/crc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/293229859/crc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/293229859/security.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/security.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/293229859" 
	@${RM} ${OBJECTDIR}/_ext/293229859/security.o.d 
	@${RM} ${OBJECTDIR}/_ext/293229859/security.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/293229859/security.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/security.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/293229859/security.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/293229859/security.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/Cisternas\ Unison\ 2.0.o: Cisternas\ Unison\ 2.0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} "${OBJECTDIR}/Cisternas Unison 2.0.o".d 
	@${RM} "${OBJECTDIR}/Cisternas Unison 2.0.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo "${OBJECTDIR}/Cisternas Unison 2.0.o"   "Cisternas Unison 2.0.c" 
	@${DEP_GEN} -d "${OBJECTDIR}/Cisternas Unison 2.0.o" 
	@${FIXDEPS} "${OBJECTDIR}/Cisternas Unison 2.0.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/DemoOutput.o: DemoOutput.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/DemoOutput.o.d 
	@${RM} ${OBJECTDIR}/DemoOutput.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/DemoOutput.o   DemoOutput.c 
	@${DEP_GEN} -d ${OBJECTDIR}/DemoOutput.o 
	@${FIXDEPS} "${OBJECTDIR}/DemoOutput.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/HardwareProfile.o: HardwareProfile.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HardwareProfile.o.d 
	@${RM} ${OBJECTDIR}/HardwareProfile.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/HardwareProfile.o   HardwareProfile.c 
	@${DEP_GEN} -d ${OBJECTDIR}/HardwareProfile.o 
	@${FIXDEPS} "${OBJECTDIR}/HardwareProfile.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/MAC_EEProm.o: MAC_EEProm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/MAC_EEProm.o.d 
	@${RM} ${OBJECTDIR}/MAC_EEProm.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/MAC_EEProm.o   MAC_EEProm.c 
	@${DEP_GEN} -d ${OBJECTDIR}/MAC_EEProm.o 
	@${FIXDEPS} "${OBJECTDIR}/MAC_EEProm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1557261057/delays.o: ../../../../CodeBank8/bsp/Microchip/delays/delays.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1557261057" 
	@${RM} ${OBJECTDIR}/_ext/1557261057/delays.o.d 
	@${RM} ${OBJECTDIR}/_ext/1557261057/delays.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1557261057/delays.o   ../../../../CodeBank8/bsp/Microchip/delays/delays.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1557261057/delays.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1557261057/delays.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/739875083/gpios.o: ../../../../CodeBank8/bsp/Microchip/gpios/gpios.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/739875083" 
	@${RM} ${OBJECTDIR}/_ext/739875083/gpios.o.d 
	@${RM} ${OBJECTDIR}/_ext/739875083/gpios.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/739875083/gpios.o   ../../../../CodeBank8/bsp/Microchip/gpios/gpios.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/739875083/gpios.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/739875083/gpios.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1390700963/spi.o: ../../../../CodeBank8/bsp/Microchip/spi/spi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1390700963" 
	@${RM} ${OBJECTDIR}/_ext/1390700963/spi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1390700963/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1390700963/spi.o   ../../../../CodeBank8/bsp/Microchip/spi/spi.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1390700963/spi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1390700963/spi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1109127202/system.o: ../../../../CodeBank8/bsp/Microchip/system/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1109127202" 
	@${RM} ${OBJECTDIR}/_ext/1109127202/system.o.d 
	@${RM} ${OBJECTDIR}/_ext/1109127202/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1109127202/system.o   ../../../../CodeBank8/bsp/Microchip/system/system.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1109127202/system.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1109127202/system.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/426468601/LCDBlocking.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Common/LCDBlocking.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/426468601" 
	@${RM} ${OBJECTDIR}/_ext/426468601/LCDBlocking.o.d 
	@${RM} ${OBJECTDIR}/_ext/426468601/LCDBlocking.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/426468601/LCDBlocking.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Common/LCDBlocking.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/426468601/LCDBlocking.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/426468601/LCDBlocking.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/426468601/TimeDelay.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Common/TimeDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/426468601" 
	@${RM} ${OBJECTDIR}/_ext/426468601/TimeDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/426468601/TimeDelay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/426468601/TimeDelay.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Common/TimeDelay.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/426468601/TimeDelay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/426468601/TimeDelay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/679473599/st7032.o: ../../../../CodeBank8/modules/st7032/st7032.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/679473599" 
	@${RM} ${OBJECTDIR}/_ext/679473599/st7032.o.d 
	@${RM} ${OBJECTDIR}/_ext/679473599/st7032.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/679473599/st7032.o   ../../../../CodeBank8/modules/st7032/st7032.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/679473599/st7032.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/679473599/st7032.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/42883563/eeprom25xx.o: ../../../../CodeBank8/modules/eeprom25xx/eeprom25xx.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/42883563" 
	@${RM} ${OBJECTDIR}/_ext/42883563/eeprom25xx.o.d 
	@${RM} ${OBJECTDIR}/_ext/42883563/eeprom25xx.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/42883563/eeprom25xx.o   ../../../../CodeBank8/modules/eeprom25xx/eeprom25xx.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/42883563/eeprom25xx.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/42883563/eeprom25xx.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1882031954/MiWi.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWi/MiWi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1882031954" 
	@${RM} ${OBJECTDIR}/_ext/1882031954/MiWi.o.d 
	@${RM} ${OBJECTDIR}/_ext/1882031954/MiWi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1882031954/MiWi.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWi/MiWi.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1882031954/MiWi.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1882031954/MiWi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1110780129/MiWiPRO.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWiPRO/MiWiPRO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1110780129" 
	@${RM} ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o.d 
	@${RM} ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MiWiPRO/MiWiPRO.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1110780129/MiWiPRO.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1110780129/MiWiPRO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1584730194/P2P.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/P2P/P2P.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1584730194" 
	@${RM} ${OBJECTDIR}/_ext/1584730194/P2P.o.d 
	@${RM} ${OBJECTDIR}/_ext/1584730194/P2P.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1584730194/P2P.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/P2P/P2P.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1584730194/P2P.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1584730194/P2P.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swacki2c.o: SW_I2C/swacki2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swacki2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swacki2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swacki2c.o   SW_I2C/swacki2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swacki2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swacki2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swckti2c.o: SW_I2C/swckti2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swckti2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swckti2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swckti2c.o   SW_I2C/swckti2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swckti2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swckti2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swgtci2c.o: SW_I2C/swgtci2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swgtci2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swgtci2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swgtci2c.o   SW_I2C/swgtci2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swgtci2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swgtci2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swgtsi2c.o: SW_I2C/swgtsi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swgtsi2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swgtsi2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swgtsi2c.o   SW_I2C/swgtsi2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swgtsi2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swgtsi2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swptci2c.o: SW_I2C/swptci2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swptci2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swptci2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swptci2c.o   SW_I2C/swptci2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swptci2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swptci2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swptsi2c.o: SW_I2C/swptsi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swptsi2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swptsi2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swptsi2c.o   SW_I2C/swptsi2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swptsi2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swptsi2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swrsti2c.o: SW_I2C/swrsti2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swrsti2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swrsti2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swrsti2c.o   SW_I2C/swrsti2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swrsti2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swrsti2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swstpi2c.o: SW_I2C/swstpi2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swstpi2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swstpi2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swstpi2c.o   SW_I2C/swstpi2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swstpi2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swstpi2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SW_I2C/swstri2c.o: SW_I2C/swstri2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/SW_I2C" 
	@${RM} ${OBJECTDIR}/SW_I2C/swstri2c.o.d 
	@${RM} ${OBJECTDIR}/SW_I2C/swstri2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/SW_I2C/swstri2c.o   SW_I2C/swstri2c.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SW_I2C/swstri2c.o 
	@${FIXDEPS} "${OBJECTDIR}/SW_I2C/swstri2c.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/Console.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/Console.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/Console.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/Console.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/Console.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/Console.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/Console.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/Console.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/EEPROM.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/EEPROM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/EEPROM.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/EEPROM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/EEPROM.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/EEPROM.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/EEPROM.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/EEPROM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/MSPI.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MSPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/MSPI.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/MSPI.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/MSPI.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/MSPI.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/MSPI.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/MSPI.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/NVM.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/NVM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/NVM.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/NVM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/NVM.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/NVM.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/NVM.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/NVM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/554139889/SymbolTime.o: ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/SymbolTime.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554139889" 
	@${RM} ${OBJECTDIR}/_ext/554139889/SymbolTime.o.d 
	@${RM} ${OBJECTDIR}/_ext/554139889/SymbolTime.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/554139889/SymbolTime.o   ../../../../microchip_solutions_v2013-06-15/Microchip/WirelessProtocols/SymbolTime.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/554139889/SymbolTime.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554139889/SymbolTime.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1782009031/MRF89XA.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/MRF89XA/MRF89XA.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1782009031" 
	@${RM} ${OBJECTDIR}/_ext/1782009031/MRF89XA.o.d 
	@${RM} ${OBJECTDIR}/_ext/1782009031/MRF89XA.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1782009031/MRF89XA.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/MRF89XA/MRF89XA.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1782009031/MRF89XA.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1782009031/MRF89XA.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/293229859/crc.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/crc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/293229859" 
	@${RM} ${OBJECTDIR}/_ext/293229859/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/293229859/crc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/293229859/crc.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/crc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/293229859/crc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/293229859/crc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/293229859/security.o: ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/security.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/293229859" 
	@${RM} ${OBJECTDIR}/_ext/293229859/security.o.d 
	@${RM} ${OBJECTDIR}/_ext/293229859/security.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/293229859/security.o   ../../../../microchip_solutions_v2013-06-15/Microchip/Transceivers/security.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/293229859/security.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/293229859/security.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/Cisternas\ Unison\ 2.0.o: Cisternas\ Unison\ 2.0.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} "${OBJECTDIR}/Cisternas Unison 2.0.o".d 
	@${RM} "${OBJECTDIR}/Cisternas Unison 2.0.o" 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"." -I"../../../../CodeBank8" -I"../../../../CodeBank8/bsp/Microchip" -I"../../../../CodeBank8/middleware" -I"../../../../CodeBank8/modules" -I"C:/microchip_solutions_v2013-06-15/Microchip" -I"C:/microchip_solutions_v2013-06-15/Microchip/Include" -I"C:/microchip_solutions_v2013-06-15/Microchip/Transceivers" -I"C:/microchip_solutions_v2013-06-15/Microchip/WirelessProtocols" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo "${OBJECTDIR}/Cisternas Unison 2.0.o"   "Cisternas Unison 2.0.c" 
	@${DEP_GEN} -d "${OBJECTDIR}/Cisternas Unison 2.0.o" 
	@${FIXDEPS} "${OBJECTDIR}/Cisternas Unison 2.0.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/MiWi_Rich_Features_Node1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/MiWi_Rich_Features_Node1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/MiWi_Rich_Features_Node1.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE)   -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/MiWi_Rich_Features_Node1.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
