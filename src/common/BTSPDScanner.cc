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
#include "BTSPDVulnerablePoint.h"
#include <GlobalNodeListAccess.h>

Define_Module(BTSPDScanner);


#define SCAN_AND_ATTACK_TIMER       1

BTSPDScanner::BTSPDScanner():
        globalNodeList(NULL),
        pAttackTime(NULL)
{

}

BTSPDScanner::~BTSPDScanner()
{

}

void BTSPDScanner::initialize()
{
    pAttackTime= new cMessage("ATTACK_TIMER", SCAN_AND_ATTACK_TIMER);
    globalNodeList = GlobalNodeListAccess().get();

    scanAndAttack();
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
    for(;;)
    {
        PeerInfo* pe=globalNodeList->getRandomPeerInfo();
        cModule * pMod= simulation.getModule(pe->getModuleID());
        cModule * pSubMod= pMod->getSubmodule("vulnerability");
        BTSPDVulnerablePoint * pVulPoint=check_and_cast<BTSPDVulnerablePoint *>(pSubMod);
        break;
    }

    scheduleAt(simTime()+1,pAttackTime );


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
