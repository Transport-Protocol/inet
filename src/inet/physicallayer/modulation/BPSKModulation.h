//
// Copyright (C) 2014 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_BPSKMODULATION_H
#define __INET_BPSKMODULATION_H

#include "inet/physicallayer/base/packetlevel/MQAMModulationBase.h"

namespace inet {

namespace physicallayer {

/**
 * This class implements binary phase-shift keying.
 *
 * http://en.wikipedia.org/wiki/Phase-shift_keying#Binary_phase-shift_keying_.28BPSK.29
 */
class INET_API BPSKModulation : public MQAMModulationBase
{
  public:
    static const BPSKModulation singleton;

  protected:
    static const std::vector<APSKSymbol> constellation;

  public:
    BPSKModulation();

    virtual void printToStream(std::ostream& stream, int level) const { stream << "BPSKModulation"; }

    virtual double calculateSER(double snir, Hz bandwidth, bps bitrate) const;
    virtual double calculateBER(double snir, Hz bandwidth, bps bitrate) const;
};

} // namespace physicallayer

} // namespace inet

#endif // ifndef __INET_BPSKMODULATION_H

