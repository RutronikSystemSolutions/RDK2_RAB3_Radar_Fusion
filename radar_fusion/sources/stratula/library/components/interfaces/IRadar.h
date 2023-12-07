/**
 * \addtogroup      IRadar
 * \brief
 * Radar component access interface
 * @{
 */
#ifndef I_RADAR_H
#define I_RADAR_H 1

#include <common/errors.h>
#include <stdint.h>
#include <universal/data_definitions.h>
#include <universal/types/IfxRfe_Types.h>


#define OPERATION_MODE_SINGLE               0x00
#define OPERATION_MODE_SLAVE_LOIN_TX1       0x02  // use TX2 and TX3
#define OPERATION_MODE_SLAVE_LOIN_TX3       0x03  // use TX1 and TX2
#define OPERATION_MODE_MASTER_LOOUT_TX1     0x10  // use TX2 and TX3
#define OPERATION_MODE_MASTER_LOOUT_TX2     0x20  // use TX1 and TX3
#define OPERATION_MODE_MASTER_LOOUT_TX3     0x30  // use TX1 and TX2
#define OPERATION_MODE_MASTER_LOOUT_TX1_TX3 0x40  // disable TX2

typedef struct
{
    uint8_t operationMode;    ///< see defines
    uint8_t txChannelOffset;  ///< Logical offset for TX1
    uint8_t rxChannelOffset;  ///< Logical offset for RX1
} IRadarConfig_t;

typedef struct
{
    uint8_t devId;            ///< control interface device id
    uint8_t dataIndex;        ///< data interface index
    uint8_t channelSwapping;  ///< swapping mode for receive channels
} BoardRadarConfig_t;


typedef struct _IRadar IRadar;
struct _IRadar
{
    /**
     * Perform reset
     * @param softReset Set to true to do a soft reset, otherwise hard reset will be performed
     * @return Strata error code
     */
    sr_t (*reset)(IRadar *self, bool softReset);

    /**
     * Initialize
     * @return Strata error code
     */
    sr_t (*initialize)(IRadar *self);

    /**
     * Loads MMIC configuration.
     *
     * @param configuration MMIC configuration structure
     * @return Strata error code
     */
    sr_t (*configure)(IRadar *self, const IfxRfe_MmicConfig *c);

    /**
     * Loads Ramp Sequence.
     *
     * @param sequence Ramp Sequence structure
     * @return Strata error code
     */
    sr_t (*loadSequence)(IRadar *self, IfxRfe_Sequence *s);

    /**
     * Performs calibration routine.
     * @return Strata error code
     */
    sr_t (*calibrate)(IRadar *self);

    /**
     * Starts execution of Ramp Sequence.
     * @return Strata error code
     */
    sr_t (*startSequence)(IRadar *self);

    /**
     * Starts the data interface
     * @return Strata error code
     */
    sr_t (*startData)(IRadar *self);

    /**
     * Stops the data interface
     * @return Strata error code
     */
    sr_t (*stopData)(IRadar *self);

    /**
     * Get index of connected data interface
     * @param index pointer to a variable to write the obtained index to
     * @return Strata error code
     */
    sr_t (*getDataIndex)(IRadar *self, uint8_t *index);

    /**
     * Get the number of configured RX channels
     * @param number pointer to a variable to write the number to
     * @return Strata error code
     */
    sr_t (*getRxChannels)(IRadar *self, uint8_t *number);

    /**
     * Enables constant frequency output on selected TX channels
     *
     * @param txMask binary mask for TX channels to enable
     * @param txPower [%] TX channel output power {0.0: 100.0}
     * @return Strata error code
     */
    sr_t (*enableConstantFrequencyMode)(IRadar *self, uint16_t txMask, float txPower);

    /**
     * Sets a constant frequency on previously enabled TX channels
     *
     * @param frequency desired frequency
     * @return Strata error code
     */
    sr_t (*setConstantFrequency)(IRadar *self, double frequency);

    /**
     * Special implementation of the IRadar interface
     * Must be set by constructor of the derived "class"
     */
    uint8_t m_bImplementation;
};


#endif /* I_RADAR_H */

/** @} */
