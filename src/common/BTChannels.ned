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

package oversim.common;

// DSL TX channel for BitTorrent InetUnderlay
channel bit_inet_dsl_tx extends ned.DatarateChannel
{
    parameters:
        delay = 30ms;
        datarate = 5.56Mbps;
}

// DSL RX channel for BitTorrent InetUnderlay
channel bit_inet_dsl_rx extends ned.DatarateChannel
{
    parameters:
        delay = 30ms;
        datarate = 13.69Mbps;
}