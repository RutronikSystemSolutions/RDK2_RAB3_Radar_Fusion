
#ifndef DATA_AVIAN_H_
#define DATA_AVIAN_H_ 1

#include <impl/PlatformInterruptDefinition.h>
#include <platform/interfaces/IData.h>


extern IData DataAvian;


void DataAvian_Constructor(IData_acquisitionStatusCallback statusCb);
void DataAvian_setBuffer(uint8_t index, uint8_t *buffer, uint32_t bufferSize);
void DataAvian_initialize(uint8_t index, const PlatformInterruptDefinition_t *irq, bool useQspi);
void DataAvian_run(void);


sr_t DataAvian_calibrationRequired(uint8_t index, double dataRate, bool *isRequired);
sr_t DataAvian_calibrate(uint8_t index);
sr_t DataAvian_configure(uint8_t index, const IDataProperties_t *dataProperties, const uint8_t *settings, uint16_t settingsSize);
sr_t DataAvian_start(uint8_t index);
sr_t DataAvian_stop(uint8_t index);
sr_t DataAvian_getStatusFlags(uint8_t index, uint32_t *flags);
sr_t DataAvian_registerCallback(IData_callback callback, void *arg);


#endif /* DATA_AVIAN_H_ */
