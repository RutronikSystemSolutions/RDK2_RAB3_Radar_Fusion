#ifndef COMMAND_HANDLER_RADAR_H_
#define COMMAND_HANDLER_RADAR_H_ 1

#include <components/interfaces/IPowerSupply.h>
#include <components/interfaces/IRadar.h>
#include <modules/interfaces/IModuleRadar.h>
#include <platform/interfaces/IProcessingRadar.h>


void CommandHandlerRadar_Constructor(void);

void CommandHandlerRadar_register_Single(IModuleRadar *module,
                                         IRadar *radar0,
                                         IProcessingRadar *processing0,
                                         IPowerSupply *powerSupply0);

void CommandHandlerRadar_register_Dual(IModuleRadar *module,
                                       IRadar *radar0, IRadar *radar1,
                                       IProcessingRadar *processing0, IProcessingRadar *processing1,
                                       IPowerSupply *powerSupply0);

void CommandHandlerRadar_register_Triple(IModuleRadar *module,
                                         IRadar *radar0, IRadar *radar1, IRadar *radar2,
                                         IProcessingRadar *processing0, IProcessingRadar *processing1,
                                         IPowerSupply *powerSupply0);

void CommandHandlerRadar_register_Highres(IModuleRadar *module,
                                          IRadar *radar0, IRadar *radar1, IRadar *radar2, IRadar *radar3, IRadar *radar4,
                                          IProcessingRadar *processing0, IProcessingRadar *processing1, IProcessingRadar *processing2, IProcessingRadar *processing3,
                                          IPowerSupply *powerSupply0, IPowerSupply *powerSupply1, IPowerSupply *powerSupply2);


#endif /* COMMAND_HANDLER_RADAR_H_ */
