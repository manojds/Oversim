//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "BTSPDScanner.h"
#include "../../inet/src/applications/BitTorrentSPD/BTSPDVulnerablePoint.h"
#include "../../inet/src/applications/BitTorrentSPD/BTThreatHandler.h"
#include <GlobalNodeListAccess.h>

Define_Module(BTSPDScanner);


#define SCAN_AND_ATTACK_TIMER       1

BTSPDScanner::BTSPDScanner():
        i_ScanInterval(100),
        globalNodeList(NULL),
        pAttackTimerMsg(NULL)

{

}

BTSPDScanner::~BTSPDScanner()
{
    delete pAttackTimerMsg;

}

void BTSPDScanner::initialize()
{
    pAttackTimerMsg= new cMessage("ATTACK_TIMER", SCAN_AND_ATTACK_TIMER);
    globalNodeList = GlobalNodeListAccess().get();

    bool bEnableScanning= par("enableScanning");
    if(bEnableScanning)
    {

        double dInfectionRate=par("infectionRate");
        i_ScanInterval=(int)(1/dInfectionRate);

        scanAndAttack();
    }
}

void BTSPDScanner::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        handleTimer(msg);
    }
    else
        throw cRuntimeError("BTSPDScanner::handleMessage - Unknown message received. kind [%d] name [%s]",msg->getKind(), msg->getName());
}

void BTSPDScanner::scanAndAttack()
{
    std::cout<<"scanAndAttack - enter"<<std::endl;

    for(;;)
    {
        PeerInfo* pe=globalNodeList->getRandomPeerInfo();
        if(pe == NULL)
            break;

        cModule * pMod= simulation.getModule(pe->getModuleID());

        cModule * pSubMod= pMod->getSubmodule("vulnerability");
        if(pSubMod == NULL)
            continue;

        BTSPDVulnerablePoint * pVulPoint=check_and_cast<BTSPDVulnerablePoint *>(pSubMod);

        if(pVulPoint->isVulnerable())
        {
            std::cout<<pMod->getFullName()<<" - is vulnerable"<<std::endl;


            std::cout<<"Attacking on - "<<pMod->getFullName()<<std::endl;

            pVulPoint->exploit();

        }

        break;
    }

    scheduleAt(simTime()+i_ScanInterval,pAttackTimerMsg );


}


void BTSPDScanner::handleTimer(cMessage *msg)
{
    if(msg ->getKind() == SCAN_AND_ATTACK_TIMER)
    {
        scanAndAttack();
    }
    else
        throw cRuntimeError("BTSPDScanner::handleTimer - Unknown message received. kind [%d] name [%s]",msg->getKind(), msg->getName());

}
