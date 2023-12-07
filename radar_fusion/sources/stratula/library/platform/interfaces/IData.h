/**
 * \addtogroup      IData
 * \brief
 * This interface supports control and configuration of a data receiver,
 * as specified by IDataProperties_t, and an implementation specific settings buffer
 * @{
 */
#ifndef IDATA_H
#define IDATA_H 1

#include <common/errors.h>
#include <stdbool.h>
#include <stdint.h>
#include <universal/data_definitions.h>


typedef struct
{
    /**
     * Informs whether calibration is required for a certain data rate
     *
     * @param index of data receiver
     * @param dataRate data rate in bits per second
     * @param isRequired out parameter to store the result
     * @return Strata error code
     */
    sr_t (*calibrationRequired)(uint8_t index, double dataRate, bool *isRequired);

    /**
     * Performs calibration routine on data interface
     *
     * @param index of data receiver
     * @return Strata error code
     */
    sr_t (*calibrate)(uint8_t index);

    /**
     * Configures the data interface to continuously receive the described data
     * This can be repeated with different configurations
     *
     * @param index of data receiver
     * @param *dataProperties pointer to memory structure containing the configuration
     * @param *settings pointer to memory buffer containing the settings
     * @param length size of memory buffer containing the settings
     * @return Strata error code
     */
    sr_t (*configure)(uint8_t index, const IDataProperties_t *dataProperties, const uint8_t *settings, uint16_t length);

    /**
     * Start the data receiving hardware
     *
     * @param index of data receiver
     * @return Strata error code
     */
    sr_t (*start)(uint8_t index);

    /**
     * Stop the data receiving hardware
     *
     * @param index of data receiver
     * @return Strata error code
     */
    sr_t (*stop)(uint8_t index);

    /**
     * Returns the internal state of the data interface
     *
     * @param index of data receiver
     * @param mask containing the status flags
     * @return Strata error code
     */
    sr_t (*getStatusFlags)(uint8_t index, uint32_t *flags);

    sr_t (*registerCallback)(IData_callback callback, void *arg);
} IData;

#endif /* IDATA_H */
/** @} */
