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

Define_Module(BitTorrentChurn);

bool BitTorrentChurn::b_TrackerCreated(false);

void BitTorrentChurn::initializeChurn()
{
    Enter_Method_Silent();

    initAddMoreTerminals = true;
    targetOverlayTerminalNum = par("targetOverlayTerminalNum");

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

    scheduleNodeCreations();
}

void BitTorrentChurn::scheduleNodeCreations()
{
    for (int i = 0; i < targetOverlayTerminalNum; i++)
    {
        scheduleAt(simTime() + distributionFunction(),
                new ChurnMessage("CreateNode"));
    }
}

void BitTorrentChurn::handleMessage(cMessage* msg)
{

    if (!msg->isSelfMessage()) 
    {
        delete msg;
        return;
    }

	if(!b_TrackerCreated)
	{
		InetUnderlayConfigurator * pConfigurator=check_and_cast<InetUnderlayConfigurator*>(underlayConfigurator);
		if(pConfigurator ==NULL)
			opp_error("BitTorrentChurn::handleMessage - underlayConfigurator is not InetUnderlayConfigurator type.");

		pConfigurator->createTrackerAndSeeder();
		b_TrackerCreated=true;
	}

    if (terminalCount >= targetOverlayTerminalNum) 
    {
        initAddMoreTerminals = false;
        underlayConfigurator->initFinished();
        delete msg;
        return;
    } 
    else
    {
        if (dynamic_cast<ChurnMessage*>(msg))
        {
            createNode();
        }
        else
            opp_error("Unexpected msg type");
    }

    delete msg;
}

void BitTorrentChurn::createNode()
{
    InetUnderlayConfigurator * pConfigurator=check_and_cast<InetUnderlayConfigurator*>(underlayConfigurator);
    if(pConfigurator ==NULL)
        opp_error("BitTorrentChurn::handleMessage - underlayConfigurator is not InetUnderlayConfigurator type.");

    //TransportAddress* ta = pConfigurator->createBTNode(type);

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
}

//Added by Manoj- BTR-011 - 2015-02-14
void BitTorrentChurn::createInitialNodes()
{
    InetUnderlayConfigurator * pConfigurator=check_and_cast<InetUnderlayConfigurator*>(underlayConfigurator);
    if(pConfigurator ==NULL)
        opp_error("BitTorrentChurn::handleMessage - underlayConfigurator is not InetUnderlayConfigurator type.");

    pConfigurator->createTrackerAndSeeder();

    //trackerAddress = pConfigurator->createBTNode("inet.applications.BitTorrent.Tracker",false);
    //pConfigurator->createBTNode("inet.applications.BitTorrent.BTHostSeeder",false);

    b_TrackerCreated=true;

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

BitTorrentChurn::BitTorrentChurn()
{
}

BitTorrentChurn::~BitTorrentChurn()
{

}
