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

#include "BitTorrentNoChurn.h"
#include <Churn_m.h>
#include <iostream>

Define_Module(BitTorrentNoChurn);

//void BitTorrentNoChurn::scheduleNodeCreations()
//{
//    std::cout<<"BitTorrentNoChurn::scheduleNodeCreations - scheduling node creations..."<<std::endl;
//
//    for (int i = 0; i < targetOverlayTerminalNum; i++)
//    {
//        scheduleAt(simTime() ,new ChurnMessage("CreateNode"));
//    }
//}

std::vector<simtime_t> BitTorrentNoChurn::getNodeStartTimes()
{
    std::vector<simtime_t> vec(targetOverlayTerminalNum);

    for (int i = 0; i < targetOverlayTerminalNum; i++)
    {
        vec[i] = simTime() ;
    }

    return vec;
}

