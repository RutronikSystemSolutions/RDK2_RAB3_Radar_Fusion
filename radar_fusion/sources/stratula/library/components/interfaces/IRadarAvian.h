/**
 * \addtogroup      IRadarAvian
 * \brief
 * Radar component access interface
 * @{
 */
#ifndef I_RADAR_AVIAN_H
#define I_RADAR_AVIAN_H 1

#include <components/interfaces/IPinsAvian.h>
#include <components/interfaces/IProtocolAvian.h>
#include <components/interfaces/IRadar.h>
#include <components/interfaces/IRegisters8_32.h>


/**
 * \brief Access interface to a radar front end device of the Avian family,
 * e.g. BGT60TR13C, BGT60ATR24C, BGT60TR13D and BGT60TR12E.
 */
typedef struct _IRadarAvian IRadarAvian;
struct _IRadarAvian
{
    IRadar b_IRadar;

    IRegisters8_32 *(*getIRegisters)(IRadarAvian *self);
    IPinsAvian *(*getIPinsAvian)(IRadarAvian *self);
    IProtocolAvian *(*getIProtocolAvian)(IRadarAvian *self);
};


#endif /* I_RADAR_AVIAN_H */

/** @} */
