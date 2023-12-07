
#ifndef DATA_LTR11_H_
#define DATA_LTR11_H_ 1

#include <components/interfaces/IPinsLtr11.h>
#include <components/interfaces/IProtocolLtr11.h>
#include <platform/interfaces/IData.h>

#define DATA_LTR11_DETECTOR_OUTPUT_VIRTUAL_ADDR 0xFF

extern IData DataLtr11;


void DataLtr11_Constructor(IData_acquisitionStatusCallback statusCb);
void DataLtr11_initialize(uint8_t index, IProtocolLtr11 *protocol, IPinsLtr11 *pins);
void DataLtr11_setBuffer(uint8_t index, uint16_t *buffer, uint32_t bufferSize);
void DataLtr11_run(void);


sr_t DataLtr11_calibrationRequired(uint8_t index, double dataRate, bool *isRequired);
sr_t DataLtr11_calibrate(uint8_t index);
sr_t DataLtr11_configure(uint8_t index, const IDataProperties_t *dataProperties, const uint8_t *settings, uint16_t settingsSize);
sr_t DataLtr11_start(uint8_t index);
sr_t DataLtr11_stop(uint8_t index);
sr_t DataLtr11_getStatusFlags(uint8_t index, uint32_t *flags);
sr_t DataLtr11_registerCallback(IData_callback callback, void *arg);


#endif /* DATA_LTR11_H_ */