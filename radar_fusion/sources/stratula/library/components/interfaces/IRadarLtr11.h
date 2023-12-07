/**
 * \addtogroup      IRadarLtr11.h
 * \brief
 * Radar component access interface
 * @{
 */

#ifndef IRADARLTR11_H_
#define IRADARLTR11_H_

#include <components/interfaces/IPinsLtr11.h>
#include <components/interfaces/IProtocolLtr11.h>
#include <components/interfaces/IRadar.h>
#include <components/interfaces/IRegisters8_16.h>


/**
 * \brief Access interface to a radar front end device of type BGT60LTR11.
 */
typedef struct _IRadarLtr11 IRadarLtr11;
struct _IRadarLtr11
{
    IRadar b_IRadar;

    IRegisters8_16 *(*getIRegisters)(IRadarLtr11 *self);
    IPinsLtr11 *(*getIPinsLtr11)(IRadarLtr11 *self);
    IProtocolLtr11 *(*getIProtocolLtr11)(IRadarLtr11 *self);
};


#endif /* IRADARLTR11_H_ */

/** @} */