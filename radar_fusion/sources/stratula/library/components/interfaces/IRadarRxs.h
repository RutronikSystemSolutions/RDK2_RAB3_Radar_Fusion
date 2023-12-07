/**
 * \addtogroup      IRadarRxs
 * \brief
 * Radar component access interface
 * @{
 */
#ifndef I_RADAR_RXS_H
#define I_RADAR_RXS_H 1

#include <components/interfaces/IPinsRxs.h>
#include <components/interfaces/IRadar.h>
#include <components/interfaces/IRegisters16.h>
#include <universal/types/Types_IRadarRxs.h>


typedef struct _IRadarRxs IRadarRxs;
struct _IRadarRxs
{
    IRadar b_IRadar;

    IPinsRxs *(*getIPinsRxs)(IRadarRxs *self);
    IRegisters16 *(*getIRegisters)(IRadarRxs *self);

    /**
     * Load firmware (if applicable) and set default values
     */
    sr_t (*setDefaults)(IRadarRxs *self);

    /**
     * Obtains OK pin status.
     *
     * @param state receives true for high and false for low
     * @return Strata error code
     */
    sr_t (*getStatus)(IRadarRxs *self, bool *state);
    /**
     * Configures DMUX pins functionality
     *
     * @param map define the functionality to be configured for all DMUX pins (see defines)
     * @return Strata error code
     */
    sr_t (*configureDmuxMap)(IRadarRxs *self, const uint8_t map[I_PINS_RXS_DMUX_COUNT]);
    /**
     * Enables output data divider.
     *
     * @param enable true for high and false for low
     * @return Strata error code
     */
    sr_t (*enableDividerOutput)(IRadarRxs *self, bool enable);
    /**
     * Enables output data CRC transmission.
     *
     * @param enable true for high and false for low
     * @return Strata error code
     */
    sr_t (*enableDataCrc)(IRadarRxs *self, bool enable);

    /**
     * Sets external trigger source for Ramp Sequence execution.
     *
     * @param src DMUX pin index
     * @return Strata error code
     */
    sr_t (*setTriggerSource)(IRadarRxs *self, uint8_t src);

    /**
     * Enables all previously configured Rx and Tx ports.
     * @return Strata error code
     */
    sr_t (*enableTxRx)(IRadarRxs *self);

    // lower level interfaces
    /** \private */
    /*
     * Adds Ramp configuration descriptor to internal memory.
     *
     * @param cd Ramp configuration descriptor structure
     * @return Strata error code
     */
    sr_t (*addConfigDescriptor)(IRadarRxs *self, ConfigDescriptor *cd);
    /** \private */
    /*
     * Adds Ramp Sequence segment descriptor to internal memory.
     *
     * @param sd Ramp Sequence segment descriptor structure
     * @return Strata error code
     */
    sr_t (*addSegmentDescriptor)(IRadarRxs *self, SegmentDescriptor *sd);
    /** \private */
    /*
     * End a Ramp Sequence.
     *
     * @return Strata error code
     */
    sr_t (*finalizeSequencer)(IRadarRxs *self);

    /**
     * Starts execution of a FW function.
     *
     * @param callcode FW call code
     * @param params sequence of FW call parameters
     * @param paramCount number of parameters
     * @return Strata error code
     */
    sr_t (*startFirmwareFunction)(IRadarRxs *self, uint16_t callcode, const uint16_t params[], uint16_t paramCount);
    /**
     * Obtains status of last executed FW function.
     *
     * @param *status pointer to memory location where to store the execution status code
     * @param *count pointer to memory location where to store the number of return values
     * @return Strata error code
     */
    sr_t (*checkFirmwareFunctionStatus)(IRadarRxs *self, uint16_t *status, uint16_t *count);
    /**
     * Obtains result of last executed FW function.
     *
     * @param retVals pointer to memory location where to store the sequence of return values
     * @param retCount number of expected return values
     * @return Strata error code
     */
    sr_t (*getFirmwareFunctionResult)(IRadarRxs *self, uint16_t retVals[], uint16_t retCount);
    /**
     * Executes a FW function and returns its result.
     *
     * @param callcode FW call code
     * @param params sequence of FW call parameters
     * @param paramCount number of parameters
     * @param retVals sequence of return values
     * @param maxRetCount number of maximum expected return values
     * @param *retCount pointer to memory location where to store the number of return values
     * @param *status pointer to memory location where to store the execution status code
     * @return Strata error code
     */
    sr_t (*executeFirmwareFunction)(IRadarRxs *self, uint16_t callcode, const uint16_t params[], uint16_t paramCount, uint16_t retVals[], uint16_t maxRetCount, uint16_t *retCount, uint16_t *status);

    /**
     * Obtains the FW version.
     *
     * @param *chipVersion pointer to memory location where to store the chip version
     * @param *fwRevision pointer to memory location where to store the firmware revision
     * @return Strata error code
     */
    sr_t (*Get_firmware_version)(IRadarRxs *self, uint8_t *chipVersion, uint32_t *fwRevision);
    /**
     * Performs temperature measurement
     *
     * @param temperature Pointer to a value where to put the temperature in Celsius.
     * @return Strata error code
     */
    sr_t (*Measure_temperature)(IRadarRxs *self, float *temperature);
    /**
     * Sets LVDS frame delay
     *
     * @param rise rise delay
     * @param fall fall delay
     * @return Strata error code
     */
    sr_t (*Set_LVDS_frame_delay)(IRadarRxs *self, double *rise, double *fall);
    /**
     * Sets Tx output power
     *
     * @param TX1_DAC_value Tx1 output gain in dB
     * @param TX2_DAC_value Tx2 output gain in dB
     * @param TX3_DAC_value Tx3 output gain in dB
     * @return Strata error code
     */
    sr_t (*Set_TX_power)(IRadarRxs *self, uint8_t TX1_DAC_value, uint8_t TX2_DAC_value, uint8_t TX3_DAC_value);
    /**
     * Enables Rx inputs
     *
     * @param RX1_enable true for enable, false for disable
     * @param RX2_enable true for enable, false for disable
     * @param RX3_enable true for enable, false for disable
     * @param RX4_enable true for enable, false for disable
     * @param LP_gain low power gain
     * @param HP_gain high power gain
     * @param DCOC_shift DCOC shift frequency
     * @param DCOC_enable true for enable, false for disable
     * @param RX_settings bitmask
     * @return Strata error code
     */
    sr_t (*Enable_RX)(IRadarRxs *self, bool RX1_enable, bool RX2_enable, bool RX3_enable, bool RX4_enable, uint8_t LP_gain, uint8_t HP_gain, uint8_t DCOC_shift, uint8_t DCOC_enable, uint8_t RX_settings);
    /**
     * Enables Tx outputs
     *
     * @param TX1_enable true for enable, false for disable
     * @param TX2_enable true for enable, false for disable
     * @param TX3_enable true for enable, false for disable
     * @param operationMode bitmask
     * @return Strata error code
     */
    sr_t (*Enable_TX)(IRadarRxs *self, bool TX1_enable, bool TX2_enable, bool TX3_enable, uint8_t operationMode);
    /**
     * Starts LVDS calibration procedure.
     * @return Strata error code
     */
    sr_t (*LVDS_calib_start)(IRadarRxs *self);
    /**
     * Stops LVDS calibration procedure.
     * @return Strata error code
     */
    sr_t (*LVDS_calib_stop)(IRadarRxs *self);
    /**
     * Set state machine to "LVDS calibration done".
     * @return Strata error code
     */
    sr_t (*LVDS_calib_fake)(IRadarRxs *self);
    /**
     * Starts execution of calibration FW function
     *
     * @param nf_min n of min frequency
     * @param nf_max n of max frequency
     * @param ndf n of delta frequency
     * @param txPhase TX phase index
     * @param subfunctions specify which subfunctions are executed
     * @param calib_run_status out value showing the calibration status after execution
     * @return Strata error code
     */
    sr_t (*Calibration)(IRadarRxs *self, int32_t nf_min, int32_t nf_calib, uint8_t tx_phase_cal_iqm_mask1, uint8_t tx_phase_cal_iqm_mask2, uint8_t ref_sel, bool cal_ref, uint8_t tx_iqm_verif, uint16_t run_flags, uint16_t *calib_run_status);
    /**
     * Configures the output of a static frequency.
     *
     * @param nf frequency setting
     * @return Strata error code
     */
    sr_t (*Static_frequency)(IRadarRxs *self, int32_t nf);
    /**
     * Starts execution of Ramp Scenario.
     *
     * @param startAddress Ramp parameter set start address
     * @param triggerSource trigger input DMUX pin id
     * @param monitoring true for enable, false for disable
     * @return Strata error code
     */
    sr_t (*Start_rmp_scenario)(IRadarRxs *self, uint16_t startAddress, uint8_t triggerSource, uint8_t monitoring);
    /**
     * Enables low power mode between sequences.
     * @return Strata error code
     */
    sr_t (*Goto_low_power)(IRadarRxs *self);
    /**
     * Enables external clock output signal.
     *
     * @param Enable_25_Clk_Out true for enable, false for disable
     * @param Enable_50_Clk_Out true for enable, false for disable
     * @return Strata error code
     */
    sr_t (*Set_clk_out)(IRadarRxs *self, bool Enable_25_Clk_Out, bool Enable_50_Clk_Out);
    /**
     * Configures a specified angle for the following modules:
     * - phase of modulation ramp segment of each Tx
     * - phase of Rx LO signal
     * - phase of test signal generator
     *
     * @param config_index selects a channel phase configuration slot
     * @param phase_angle phase angle setting
     * @return Strata error code
     */
    sr_t (*Set_phase)(IRadarRxs *self, uint8_t config_index, uint16_t phase_angle);
    /*
     * Writes the charge pump current factor A value to the memory address.
     * Charge pump current factor defines how much current is used for PLL, used to adjust PLL bandwidth
     *
     * @param icpA charge pump current factor A
     * @return Strata error code
     */
    sr_t (*setChargePumpCurrentA)(IRadarRxs *self, uint16_t icpA);
    /*
     * Writes the charge pump current factor C value to the memory address.
     * Charge pump current factor defines how much current is used for PLL, used to adjust PLL bandwidth
     *
     * @param icpC charge pump current factor C
     * @return Strata error code
     */
    sr_t (*setChargePumpCurrentC)(IRadarRxs *self, uint16_t icpC);
    /*
     * Writes the mode of charge pump current setting to the memory address
     *
     * @param mode offset charge pump current setting: 0 = down mode, 1 = up mode
     * @return Strata error code
     */
    sr_t (*setChargePumpOffset)(IRadarRxs *self, uint16_t mode);
    /*
     * CRC checksum calculation over memory block of predefined parameters
     *
     * @return Strata error code
     */
    sr_t (*calculateParameterCrc)(IRadarRxs *self);
};


#endif /* I_RADAR_RXS_H */

/** @} */
