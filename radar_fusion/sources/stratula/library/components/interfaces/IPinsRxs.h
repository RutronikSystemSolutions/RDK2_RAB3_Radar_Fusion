/**
 * \addtogroup      IPinsRxs
 * \brief
 * Radar component GPIO access interface
 * @{
 */
#ifndef I_PINS_RXS_H
#define I_PINS_RXS_H 1

#include <common/errors.h>
#include <universal/types/Types_IRadarRxs.h>


typedef struct _IPinsRxs IPinsRxs;
struct _IPinsRxs
{
    /**
    * Sets state of radar reset pin.
    *
    * @param state true for high and false for low
    * @return Strata error code
    */
    sr_t (*setResetPin)(IPinsRxs *self, bool state);
    /**
    * Perform a reset of the Rxs
    *  This sets the reset pin low and then high respecting the correct timing.
    *
    * @return Strata error code
    */
    sr_t (*reset)(IPinsRxs *self);
    /**
    * Obtains state of radar OK pin.
    * @param state State of the pin, true for high and false for low
    * @return Strata error code
    */
    sr_t (*getOkPin)(IPinsRxs *self, bool *state);
    /**
    * Configures read or write access to radar DMUX pin.
    *
    * @param index DMUX pin index
    * @param flags bit-wise setting of features
    * @return Strata error code
    */
    sr_t (*configureDmuxPin)(IPinsRxs *self, uint8_t index, uint8_t flags);
    /**
    * Obtains state of radar DMUX pin.
    *
    * @param index DMUX pin index
    * @param state State of the pin, true for high and false for low
    * @return Strata error code
    */
    sr_t (*getDmuxPin)(IPinsRxs *self, uint8_t index, bool *state);
    /**
    * Sets state of radar DMUX pin.
    *
    * @param index DMUX pin index
    * @param state true for high and false for low
    * @return Strata error code
    */
    sr_t (*setDmuxPin)(IPinsRxs *self, uint8_t index, bool state);
};


#endif /* I_PINS_RXS_H */

/** @} */
