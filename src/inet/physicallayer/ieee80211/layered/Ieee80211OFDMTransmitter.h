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

#ifndef __INET_IEEE80211OFDMTRANSMITTER_H
#define __INET_IEEE80211OFDMTRANSMITTER_H

#include "inet/physicallayer/contract/layered/IEncoder.h"
#include "inet/physicallayer/contract/layered/IModulator.h"
#include "inet/physicallayer/contract/layered/IPulseShaper.h"
#include "inet/physicallayer/contract/layered/IDigitalAnalogConverter.h"
#include "inet/physicallayer/contract/ITransmitter.h"
#include "inet/physicallayer/base/APSKModulationBase.h"
#include "inet/physicallayer/ieee80211/mode/Ieee80211OFDMMode.h"

namespace inet {

namespace physicallayer {

class INET_API Ieee80211OFDMTransmitter : public ITransmitter, public cSimpleModule
{
    // TODO: copy
    public:
        enum LevelOfDetail
        {
            PACKET_DOMAIN,
            BIT_DOMAIN,
            SYMBOL_DOMAIN,
            SAMPLE_DOMAIN,
        };

    protected:
        LevelOfDetail levelOfDetail;
        mutable const Ieee80211OFDMMode *mode = nullptr;
        const IEncoder *signalEncoder = nullptr;
        const IEncoder *dataEncoder = nullptr;
        const IModulator *signalModulator = nullptr;
        const IModulator *dataModulator = nullptr;
        const IPulseShaper *pulseShaper = nullptr;
        const IDigitalAnalogConverter *digitalAnalogConverter = nullptr;
        bool isCompliant;

        Hz channelSpacing;
        Hz carrierFrequency;
        W power;

    protected:
        virtual int numInitStages() const { return NUM_INIT_STAGES; }
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg) { throw cRuntimeError("This module doesn't handle self messages"); }

        const ITransmissionPacketModel *createSignalFieldPacketModel(const ITransmissionPacketModel *completePacketModel) const;
        const ITransmissionPacketModel *createDataFieldPacketModel(const ITransmissionPacketModel *completePacketModel) const;
        virtual const ITransmissionPacketModel *createPacketModel(const cPacket *macFrame) const;
        const ITransmissionBitModel *createBitModel(const ITransmissionBitModel *signalFieldBitModel, const ITransmissionBitModel *dataFieldBitModel, const ITransmissionPacketModel *packetModel) const;
        const ITransmissionSampleModel *createSampleModel(const ITransmissionSymbolModel *symbolModel) const;
        const ITransmissionSymbolModel *createSymbolModel(const ITransmissionSymbolModel *signalFieldSymbolModel, const ITransmissionSymbolModel *dataFieldSymbolModel) const;
        const ITransmissionAnalogModel* createAnalogModel(const ITransmissionPacketModel *packetModel, const ITransmissionBitModel *bitModel, const ITransmissionSymbolModel *symbolModel, const ITransmissionSampleModel *sampleModel) const;
        const ITransmissionAnalogModel* createScalarAnalogModel(const ITransmissionPacketModel *packetModel, const ITransmissionBitModel *bitModel) const;

        BitVector *serialize(const cPacket* packet) const;
        void encodeAndModulate(const ITransmissionPacketModel* packetModel, const ITransmissionBitModel *&fieldBitModel, const ITransmissionSymbolModel *&fieldSymbolModel, const IEncoder *encoder, const IModulator *modulator, bool isSignalField) const;
        void padding(BitVector *serializedPacket, unsigned int dataBitsLength, uint8_t rate) const;

    public:
        virtual ~Ieee80211OFDMTransmitter();

        virtual const ITransmission *createTransmission(const IRadio *radio, const cPacket *packet, const simtime_t startTime) const;
        virtual const IEncoder *getEncoder() const { return dataEncoder; }
        virtual const IModulator *getModulator() const { return dataModulator; }
        virtual const IPulseShaper *getPulseShaper() const{ return pulseShaper; }
        virtual const IDigitalAnalogConverter *getDigitalAnalogConverter() const { return digitalAnalogConverter; }
        virtual W getMaxPower() const { return power; }
        const Hz getBandwidth() const { return mode->getBandwidth(); }
        const Hz getCarrierFrequency() const { return carrierFrequency; }
        const Hz getCarrierSpacing() const { return mode->getChannelSpacing(); }
        virtual void printToStream(std::ostream& stream) const { stream << "Ieee80211OFDMTransmitter"; }
};

} // namespace physicallayer
} // namespace inet

#endif /* __INET_IEEE80211OFDMTRANSMITTER_H */
