
#ifndef DATA_ATR22_H_
#define DATA_ATR22_H_ 1

#include <components/radar/atr22/ProtocolAtr22.h>
#include <impl/PlatformInterruptDefinition.h>
#include <platform/interfaces/IData.h>


extern IData DataAtr22;


void DataAtr22_Constructor(IData_acquisitionStatusCallback statusCb);
void DataAtr22_initialize(uint8_t index, IProtocolAtr22 *protocol, const PlatformInterruptDefinition_t *irq);
void DataAtr22_setBuffer(uint8_t index, uint16_t *buffer, uint32_t bufferSize);
void DataAtr22_run(void);


sr_t DataAtr22_calibrationRequired(uint8_t index, double dataRate, bool *isRequired);
sr_t DataAtr22_calibrate(uint8_t index);
sr_t DataAtr22_configure(uint8_t index, const IDataProperties_t *dataProperties, const uint8_t *settings, uint16_t settingsSize);
sr_t DataAtr22_start(uint8_t index);
sr_t DataAtr22_stop(uint8_t index);
sr_t DataAtr22_getStatusFlags(uint8_t index, uint32_t *flags);
sr_t DataAtr22_registerCallback(IData_callback callback, void *arg);


#endif /* DATA_ATR22_H_ */
