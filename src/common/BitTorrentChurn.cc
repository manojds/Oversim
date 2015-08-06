//
// Copyright 2009 Konstantinos V. Katsaros
//                              ntinos@aueb.gr
//                              http://mm.aueb.gr/~katsaros
//

// This file is part of BitTorrent Implementation for OMNeT++.

//    BitTorrent Implementation for OMNeT++ is free software: you can redistribute
//	  it and/or modify it under the terms of the GNU General Public License as
//    published by the Free Software Foundation, either version 2 of the License,
//	  or (at your option) any later version.

//    BitTorrent Implementation for OMNeT++ is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with BitTorrent Implementation for OMNeT++.
//    If not, see <http://www.gnu.org/licenses/>.


#include "BitTorrentChurn.h"
#include "GlobalStatisticsAccess.h"
//following includes added by Manoj
#include <Churn_m.h>
#include <UnderlayConfigurator.h>
#include <../underlay/inetunderlay/InetUnderlayConfigurator.h>
#include <iostream>
#include "../BitTorrent/BTPeerWireBase.h"
#include <GlobalNodeList.h>
#include <GlobalNodeListAccess.h>

Define_Module(BitTorrentChurn);

#define CREATE_TRACKER_MSG_TYPE     1
#define START_NODE_MSG_TYPE         2

void BitTorrentChurn::initializeChurn()
{
    Enter_Method_Silent();

    initAddMoreTerminals = true;
    targetOverlayTerminalNum = par("targetOverlayTerminalNum");

    createAllAtBegining = par("createAllNodesAtBegining");

    bittorrentDistName = std::string(
            par("bittorrentDistName").stdstringValue());

    bittorrentDistPar1 = par("bittorrentDistPar1");

    if (bittorrentDistName == "decreasingExponentialArrivalRate")
        bittorrentDistPar2 = targetOverlayTerminalNum / bittorrentDistPar1;
    else
        bittorrentDistPar2 = par("bittorrentDistPar2");

    if (bittorrentDistPar1 <= 0) {
        opp_error("BitTorrentChurn currently only works with "
                "bittorrentDistPar1 > 0");
    }

    if (bittorrentDistPar2 <= 0) {
        opp_error("BitTorrentChurn currently only works with "
                "bittorrentDistPar2 > 0 ");
    }

    globalStatistics = GlobalStatisticsAccess().get();

    lastCreate = simTime();

    terminalCount = 0;

    //function added by Manoj BTR-011 - 2015-05-02
    if( idx == 0)
    {
        scheduleAt(simTime() ,
                new cMessage("CREATE_TRACKER_MSG_TYPE",
                        CREATE_TRACKER_MSG_TYPE) );
    }

    scheduleNodeCreations();
}

void BitTorrentChurn::scheduleNodeCreations()
{
//    for (int i = 0; i < targetOverlayTerminalNum; i++)
//    {
//        scheduleAt(simTime() + distributionFunction(),
//                new ChurnMessage("CreateNode"));
//    }

    vector<simtime_t> vec = getNodeStartTimes();

    for (unsigned int i = 0 ; i < vec.size() ; ++i)
    {

        if (createAllAtBegining)
        {
            createNode(vec[i]);
        }
        else
        {
            scheduleAt(vec[i], new ChurnMessage("CreateNode"));
        }


    }
}

std::vector<simtime_t> BitTorrentChurn::getNodeStartTimes()
{
    std::vector<simtime_t> vec(targetOverlayTerminalNum);

    for (int i = 0; i < targetOverlayTerminalNum; i++)
    {
        vec[i] = simTime() + distributionFunction();
    }

    return vec;

}

void BitTorrentChurn::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage())
    {
        //function added by Manoj BTR-011 - 2015-05-02
        if ( msg->getKind() ==  CREATE_TRACKER_MSG_TYPE)
        {
            createTracker();
        }
        else if (msg->getKind() == START_NODE_MSG_TYPE )
        {
            startNode((cModule*)msg->getContextPointer());

        }

        else if ( msg->getKind() == 0 )
        {
            if (dynamic_cast<ChurnMessage*>(msg) == 0)
            {
                opp_error("Unexpected msg type received for BitTorrentChurn");
            }


            createNode(simTime());
        }
        else
        {
            opp_error("Unexpected msg type received for BitTorrentChurn");
        }
    }
    else
    {
        opp_error("Unexpected msg type received for BitTorrentChurn");
    }

    delete msg;
}

void BitTorrentChurn::createTracker()
{
    InetUnderlayConfigurator * pConfigurator=check_and_cast<InetUnderlayConfigurator*>(underlayConfigurator);
    if(pConfigurator ==NULL)
        opp_error("BitTorrentChurn::handleMessage - underlayConfigurator is not InetUnderlayConfigurator type.");

    std::cout<<"BitTorrentChurn::createTracker - creating tracker"<<std::endl;
    pConfigurator->createTracker();
}

void BitTorrentChurn::createNode(simtime_t startTime)
{
    if (terminalCount >= targetOverlayTerminalNum)
    {
        initAddMoreTerminals = false;
        underlayConfigurator->initFinished();
    }
    else
    {
        executeCreateNodeReq(startTime);
    }
}

void BitTorrentChurn::executeCreateNodeReq(simtime_t startTime)
{
    InetUnderlayConfigurator * pConfigurator=check_and_cast<InetUnderlayConfigurator*>(underlayConfigurator);
    if(pConfigurator ==NULL)
        opp_error("BitTorrentChurn::handleMessage - underlayConfigurator is not InetUnderlayConfigurator type.");

    TransportAddress* ta = pConfigurator->createNode(type);

    delete ta;

	//Commented by MAnoj
//    RECORD_STATS(globalStatistics->recordOutVector(
//        "BaseOverlay: Time between creates",
//        simulation.simTime() - lastCreate));
//
//    lastCreate = simulation.simTime();

	//Code added instead of above added by Manoj
	SimTime smGap=simTime() - lastCreate;
	double  dGap=smGap.dbl();
    RECORD_STATS(globalStatistics->recordOutVector(
        "BaseOverlay: Time between creates",
        dGap));

    lastCreate = simTime();

    GlobalNodeList* globalNodeList = GlobalNodeListAccess().get();

    PeerInfo* pInfo = globalNodeList->getPeerInfo(*ta);


    cModule * pMod = simulation.getModule(pInfo->getModuleID());

    cMessage* pMsg = new cMessage("START_NODE_MSG_TYPE", START_NODE_MSG_TYPE);

    pMsg->setContextPointer(pMod);

    scheduleAt(startTime ,pMsg );



}

void BitTorrentChurn::startNode(cModule * _pMod)
{
    cModule * pSubMod = _pMod->getSubmodule("peerWire");
    BTPeerWireBase* pPeerWire = check_and_cast<BTPeerWireBase*>(pSubMod);

    pPeerWire->startNodeAt(simTime());
}



double BitTorrentChurn::distributionFunction()
{

    if (bittorrentDistName == "decreasingExponentialArrivalRate") 
    {
	return exponentialDecayArrivalRate();
    }
    else 
    {
        opp_error("BitTorrentChurn::distribution function: Invalid value "
                  "for parameter bittorrentDistName!");
    }

    return 0;
}

//Getting a random value for the CDF (integral) of the exponential decay distribution and inverting (finding t value)
double BitTorrentChurn::exponentialDecayArrivalRate()
{
	double val = -bittorrentDistPar2*log(1- (dblrand()/(bittorrentDistPar1*bittorrentDistPar2)));
	std::cerr<<"Time= "<<val<<endl;
	return val;
}


void BitTorrentChurn::updateDisplayString()
{
    char buf[80];
    sprintf(buf, "bittorrent churn");
    getDisplayString().setTagArg("t", 0, buf);
}

BitTorrentChurn::BitTorrentChurn():
        createAllAtBegining(false)
{
}

BitTorrentChurn::~BitTorrentChurn()
{

}
