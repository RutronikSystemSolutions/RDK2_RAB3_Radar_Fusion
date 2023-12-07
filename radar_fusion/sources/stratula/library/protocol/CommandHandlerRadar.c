#include "CommandHandlerRadar.h"

#include "RequestHandler.h"
#include "commands/Commands_IModuleRadar.h"
#include "commands/Commands_IPowerSupply.h"
#include "commands/Commands_IProcessingRadar.h"
#include "commands/Commands_IRadar.h"


static ICommands m_commandsIModuleRadar;
static ICommands m_commandsIRadar;
static ICommands m_commandsIProcessingRadar;
static ICommands m_commandsIPowerSupply;


void CommandHandlerRadar_Constructor(void)
{
    Commands_IModuleRadar_Constructor(&m_commandsIModuleRadar);
    Commands_IRadar_Constructor(&m_commandsIRadar);
    Commands_IProcessingRadar_Constructor(&m_commandsIProcessingRadar);
    Commands_IPowerSupply_Constructor(&m_commandsIPowerSupply);

    RequestHandler_registerModuleImplementation(&m_commandsIModuleRadar);
    RequestHandler_registerComponentImplementation(&m_commandsIRadar);
    RequestHandler_registerComponentImplementation(&m_commandsIProcessingRadar);
    RequestHandler_registerComponentImplementation(&m_commandsIPowerSupply);
}

void CommandHandlerRadar_register_Single(IModuleRadar *module,
                                         IRadar *radar0,
                                         IProcessingRadar *processing0,
                                         IPowerSupply *powerSupply0)
{
    Commands_IModuleRadar_registerModule(module);
    Commands_IRadar_register(radar0);
    Commands_IProcessingRadar_registerIProcessingRadar(processing0);
    Commands_IPowerSupply_register(powerSupply0);
}

void CommandHandlerRadar_register_Dual(IModuleRadar *module,
                                       IRadar *radar0, IRadar *radar1,
                                       IProcessingRadar *processing0, IProcessingRadar *processing1,
                                       IPowerSupply *powerSupply0)
{
    Commands_IModuleRadar_registerModule(module);
    Commands_IRadar_register(radar0);
    Commands_IRadar_register(radar1);
    Commands_IProcessingRadar_registerIProcessingRadar(processing0);
    Commands_IProcessingRadar_registerIProcessingRadar(processing1);
    Commands_IPowerSupply_register(powerSupply0);
}

void CommandHandlerRadar_register_Triple(IModuleRadar *module,
                                         IRadar *radar0, IRadar *radar1, IRadar *radar2,
                                         IProcessingRadar *processing0, IProcessingRadar *processing1,
                                         IPowerSupply *powerSupply0)
{
    Commands_IModuleRadar_registerModule(module);
    Commands_IRadar_register(radar0);
    Commands_IRadar_register(radar1);
    Commands_IRadar_register(radar2);
    Commands_IProcessingRadar_registerIProcessingRadar(processing0);
    Commands_IProcessingRadar_registerIProcessingRadar(processing1);
    Commands_IPowerSupply_register(powerSupply0);
}

void CommandHandlerRadar_register_Highres(IModuleRadar *module,
                                          IRadar *radar0, IRadar *radar1, IRadar *radar2, IRadar *radar3, IRadar *radar4,
                                          IProcessingRadar *processing0, IProcessingRadar *processing1, IProcessingRadar *processing2, IProcessingRadar *processing3,
                                          IPowerSupply *powerSupply0, IPowerSupply *powerSupply1, IPowerSupply *powerSupply2)
{
    Commands_IModuleRadar_registerModule(module);
    Commands_IRadar_register(radar0);
    Commands_IRadar_register(radar1);
    Commands_IRadar_register(radar2);
    Commands_IRadar_register(radar3);
    Commands_IRadar_register(radar4);
    Commands_IProcessingRadar_registerIProcessingRadar(processing0);
    Commands_IProcessingRadar_registerIProcessingRadar(processing1);
    Commands_IProcessingRadar_registerIProcessingRadar(processing2);
    Commands_IProcessingRadar_registerIProcessingRadar(processing3);
    Commands_IPowerSupply_register(powerSupply0);
    Commands_IPowerSupply_register(powerSupply1);
    Commands_IPowerSupply_register(powerSupply2);
}
