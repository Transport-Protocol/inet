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

#include "inet/physicallayer/common/bitlevel/LayeredReceptionDecision.h"

namespace inet {

namespace physicallayer {

LayeredReceptionDecision::LayeredReceptionDecision(const IReception *reception, const ReceptionIndication *indication, const IReceptionPacketModel *packetModel, const IReceptionBitModel *bitModel, const IReceptionSymbolModel *symbolModel, const IReceptionSampleModel *sampleModel, const IReceptionAnalogModel *analogModel, bool isReceptionPossible, bool isReceptionAttempted, bool isReceptionSuccessful) :
    ReceptionDecision(reception, indication, isReceptionPossible, isReceptionAttempted, isReceptionSuccessful),
    packetModel(packetModel),
    bitModel(bitModel),
    symbolModel(symbolModel),
    sampleModel(sampleModel),
    analogModel(analogModel)
{
}

LayeredReceptionDecision::~LayeredReceptionDecision()
{
    delete packetModel->getPacket();
    delete packetModel;
    delete bitModel;
    delete symbolModel;
    delete sampleModel;
    delete analogModel;
}

void LayeredReceptionDecision::printToStream(std::ostream& stream, int level) const
{
    stream << "LayeredReceptionDecision, "
           << "packetModel = " << packetModel << ", "
           << "bitModel = " << bitModel << ", "
           << "symbolModel = " << symbolModel << ", "
           << "sampleModel = " << sampleModel << ", "
           << "analogModel = " << analogModel;
}

const cPacket *LayeredReceptionDecision::getPhyFrame() const
{
    return packetModel->getPacket();
}

const cPacket *LayeredReceptionDecision::getMacFrame() const
{
    return packetModel->getPacket()->getEncapsulatedPacket();
}
} /* namespace physicallayer */
} /* namespace inet */

