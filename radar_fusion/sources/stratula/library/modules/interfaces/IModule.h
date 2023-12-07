/**
 * \addtogroup        ModuleInterface
 * \brief Access functions for basic module functionality
 *
 * Functions and state identifiers for generic device data callback handling. Includes routines for
 * configuration, reset, measurements and callback registration of post processing
 * functions.
 *
 * @{
 */

#ifndef I_MODULE_H
#define I_MODULE_H 1

#include <common/errors.h>
#include <stdbool.h>
#include <stdint.h>
#include <universal/data_definitions.h>


#define MODULE_UNCONFIGURED (0)
#define MODULE_STOPPED      (1)
#define MODULE_READY        (2)
#define MODULE_MEASURING    (3)

#define MEASUREMENT_MODE_MANUAL     (0)
#define MEASUREMENT_MODE_TIMER      (1)
#define MEASUREMENT_MODE_CONTINUOUS (2)


typedef struct _IModule IModule;

struct _IModule
{
    /**
     * Configure the module
     * @return Strata error code
     */
    sr_t (*configure)(IModule *self);

    /**
     * Reset the module (undo setup)
     * @return Strata error code
     */
    sr_t (*reset)(IModule *self);

    /**
     * Start sequential measurements
     * @param measurementCycle  time in seconds between measurements.
     * If 0 , measurements shall be manually triggered by calling doMeasurement().
     * @return Strata error code
     */
    sr_t (*startMeasurements)(IModule *self, double measurementCycle);

    /**
     * Stop measurements
     * @return Strata error code
     */
    sr_t (*stopMeasurements)(IModule *self);

    /**
     * Perform a single measurement
     * @return Strata error code
     */
    sr_t (*doMeasurement)(IModule *self);

    /**
     * Register a callback for processing the data-measurement
     */
    void (*registerDataCallback)(IModule *self, IProcessing_DataCallback dataCallback, void *arg);
};


#endif /* I_MODULE_H */
/** @} */
