//
// Copyright (C) 2015 OpenSim Ltd.
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

#include "inet/physicallayer/ieee80211/mode/Ieee80211HTMode.h"
#include "inet/physicallayer/ieee80211/mode/Ieee80211HTCode.h"

namespace inet {
namespace physicallayer {

Ieee80211HTMode::Ieee80211HTMode(const Ieee80211HTPreambleMode* preambleMode, const Ieee80211HTDataMode* dataMode, const Hz carrierFrequency) :
        preambleMode(preambleMode),
        dataMode(dataMode),
        carrierFrequency(carrierFrequency)
{
}

Ieee80211HTModeBase::Ieee80211HTModeBase(unsigned int modulationAndCodingScheme, unsigned int numberOfSpatialStreams, const Hz bandwidth, GuardIntervalType guardIntervalType) :
        bandwidth(bandwidth),
        guardIntervalType(guardIntervalType),
        mcsIndex(modulationAndCodingScheme),
        numberOfSpatialStreams(numberOfSpatialStreams),
        netBitrate(bps(NaN)),
        grossBitrate(bps(NaN))
{
}

Ieee80211HTPreambleMode::Ieee80211HTPreambleMode(const Ieee80211HTSignalMode* highThroughputSignalMode, const Ieee80211OFDMSignalMode *legacySignalMode, HighTroughputPreambleFormat preambleFormat, unsigned int numberOfSpatialStream) :
        highThroughputSignalMode(highThroughputSignalMode),
        legacySignalMode(legacySignalMode),
        preambleFormat(preambleFormat),
        numberOfHTLongTrainings(computeNumberOfHTLongTrainings(computeNumberOfSpaceTimeStreams(numberOfSpatialStream)))
{
}

Ieee80211HTSignalMode::Ieee80211HTSignalMode(unsigned int modulationAndCodingScheme, const Ieee80211OFDMModulation *modulation, const Ieee80211HTCode *code, const Hz bandwidth, GuardIntervalType guardIntervalType) :
        Ieee80211HTModeBase(modulationAndCodingScheme, 1, bandwidth, guardIntervalType),
        modulation(modulation),
        code(code)
{
}

Ieee80211HTSignalMode::Ieee80211HTSignalMode(unsigned int modulationAndCodingScheme, const Ieee80211OFDMModulation* modulation, const Ieee80211ConvolutionalCode *convolutionalCode, const Hz bandwidth, GuardIntervalType guardIntervalType) :
        Ieee80211HTModeBase(modulationAndCodingScheme, 1, bandwidth, guardIntervalType),
        modulation(modulation),
        code(Ieee80211HTCompliantCodes::getCompliantCode(convolutionalCode, modulation, nullptr, nullptr, nullptr, bandwidth, false))
{
}


Ieee80211HTDataMode::Ieee80211HTDataMode(const Ieee80211HTMCS *modulationAndCodingScheme, unsigned int mcsIndex, const Ieee80211OFDMModulation* stream1Modulation, const Ieee80211OFDMModulation* stream2Modulation, const Ieee80211OFDMModulation* stream3Modulation, const Ieee80211OFDMModulation* stream4Modulation, const Hz bandwidth, GuardIntervalType guardIntervalType) :
        Ieee80211HTModeBase(mcsIndex, computeNumberOfSpatialStreams(stream1Modulation,stream2Modulation, stream3Modulation, stream4Modulation), bandwidth, guardIntervalType),
        modulationAndCodingScheme(modulationAndCodingScheme),
        numberOfBccEncoders(computeNumberOfBccEncoders())
{
}

Ieee80211HTMCS::Ieee80211HTMCS(unsigned int mcsIndex, const Ieee80211HTCode* code, const Ieee80211OFDMModulation* stream1Modulation, const Ieee80211OFDMModulation* stream2Modulation, const Ieee80211OFDMModulation* stream3Modulation, const Ieee80211OFDMModulation* stream4Modulation) :
    mcsIndex(mcsIndex),
    stream1Modulation(stream1Modulation),
    stream2Modulation(stream2Modulation),
    stream3Modulation(stream3Modulation),
    stream4Modulation(stream4Modulation),
    code(code)
{
}

Ieee80211HTMCS::Ieee80211HTMCS(unsigned int mcsIndex, const Ieee80211OFDMModulation* stream1Modulation, const Ieee80211OFDMModulation* stream2Modulation, const Ieee80211OFDMModulation* stream3Modulation, const Ieee80211OFDMModulation* stream4Modulation, const Ieee80211ConvolutionalCode* convolutionalCode, Hz bandwidth) :
    mcsIndex(mcsIndex),
    stream1Modulation(stream1Modulation),
    stream2Modulation(stream2Modulation),
    stream3Modulation(stream3Modulation),
    stream4Modulation(stream4Modulation),
    code(Ieee80211HTCompliantCodes::getCompliantCode(convolutionalCode, stream1Modulation, stream2Modulation, stream3Modulation, stream4Modulation, bandwidth)),
    bandwidth(bandwidth)
{
}

Ieee80211HTMCS::Ieee80211HTMCS(unsigned int mcsIndex, const Ieee80211OFDMModulation* stream1Modulation, const Ieee80211OFDMModulation* stream2Modulation, const Ieee80211OFDMModulation* stream3Modulation, const Ieee80211ConvolutionalCode* convolutionalCode, Hz bandwidth) :
    mcsIndex(mcsIndex),
    stream1Modulation(stream1Modulation),
    stream2Modulation(stream2Modulation),
    stream3Modulation(stream3Modulation),
    stream4Modulation(nullptr),
    code(Ieee80211HTCompliantCodes::getCompliantCode(convolutionalCode, stream1Modulation, stream2Modulation, stream3Modulation, stream4Modulation, bandwidth)),
    bandwidth(bandwidth)
{
}

Ieee80211HTMCS::Ieee80211HTMCS(unsigned int mcsIndex, const Ieee80211OFDMModulation* stream1Modulation, const Ieee80211OFDMModulation* stream2Modulation, const Ieee80211ConvolutionalCode* convolutionalCode, Hz bandwidth) :
    mcsIndex(mcsIndex),
    stream1Modulation(stream1Modulation),
    stream2Modulation(stream2Modulation),
    stream3Modulation(nullptr),
    stream4Modulation(nullptr),
    code(Ieee80211HTCompliantCodes::getCompliantCode(convolutionalCode, stream1Modulation, stream2Modulation, stream3Modulation, stream4Modulation, bandwidth)),
    bandwidth(bandwidth)
{
}

Ieee80211HTMCS::Ieee80211HTMCS(unsigned int mcsIndex, const Ieee80211OFDMModulation* stream1Modulation, const Ieee80211ConvolutionalCode* convolutionalCode, Hz bandwidth) :
    mcsIndex(mcsIndex),
    stream1Modulation(stream1Modulation),
    stream2Modulation(nullptr),
    stream3Modulation(nullptr),
    stream4Modulation(nullptr),
    code(Ieee80211HTCompliantCodes::getCompliantCode(convolutionalCode, stream1Modulation, stream2Modulation, stream3Modulation, stream4Modulation, bandwidth)),
    bandwidth(bandwidth)
{
}

bps Ieee80211HTSignalMode::computeGrossBitrate() const
{
    unsigned int numberOfCodedBitsPerSymbol = modulation->getSubcarrierModulation()->getCodeWordSize() * getNumberOfDataSubcarriers();
    if (guardIntervalType == HT_GUARD_INTERVAL_LONG)
        return bps(numberOfCodedBitsPerSymbol / getSymbolInterval());
    else if (guardIntervalType == HT_GUARD_INTERVAL_SHORT)
        return bps(numberOfCodedBitsPerSymbol / getShortGISymbolInterval());
    else
        throw cRuntimeError("Unknown guard interval type");
}

bps Ieee80211HTSignalMode::computeNetBitrate() const
{
    return computeGrossBitrate() * code->getForwardErrorCorrection()->getCodeRate();
}

bps Ieee80211HTDataMode::computeGrossBitrate() const
{
    unsigned int numberOfCodedBitsPerSubcarrierSum = computeNumberOfCodedBitsPerSubcarrierSum();
    unsigned int numberOfCodedBitsPerSymbol = numberOfCodedBitsPerSubcarrierSum * getNumberOfDataSubcarriers();
    if (guardIntervalType == HT_GUARD_INTERVAL_LONG)
        return bps(numberOfCodedBitsPerSymbol / getSymbolInterval());
    else if (guardIntervalType == HT_GUARD_INTERVAL_SHORT)
        return bps(numberOfCodedBitsPerSymbol / getShortGISymbolInterval());
    else
        throw cRuntimeError("Unknown guard interval type");
}

bps Ieee80211HTDataMode::computeNetBitrate() const
{
    return computeGrossBitrate() * getCode()->getForwardErrorCorrection()->getCodeRate();
}

bps Ieee80211HTModeBase::getNetBitrate() const
{
    if (isNaN(netBitrate.get()))
        netBitrate = computeNetBitrate();
    return netBitrate;
}

bps Ieee80211HTModeBase::getGrossBitrate() const
{
    if (isNaN(grossBitrate.get()))
        grossBitrate = computeGrossBitrate();
    return grossBitrate;
}

int Ieee80211HTSignalMode::getBitLength() const
{
    return
        getMCSLength() +
        getCBWLength() +
        getHTLengthLength() +
        getSmoothingLength() +
        getNotSoundingLength() +
        getReservedLength() +
        getAggregationLength() +
        getSTBCLength() +
        getFECCodingLength() +
        getShortGILength() +
        getNumOfExtensionSpatialStreamsLength() +
        getCRCLength() +
        getTailBitsLength();
}

unsigned int Ieee80211HTPreambleMode::computeNumberOfSpaceTimeStreams(unsigned int numberOfSpatialStreams) const
{
    // Table 20-12—Determining the number of space-time streams
    return numberOfSpatialStreams + highThroughputSignalMode->getSTBC();
}

unsigned int Ieee80211HTPreambleMode::computeNumberOfHTLongTrainings(unsigned int numberOfSpaceTimeStreams) const
{
    // If the transmitter is providing training for exactly the space-time
    // streams (spatial mapper inputs) used for the transmission of the PSDU,
    // the number of training symbols, N_LTF, is equal to the number of space-time
    // streams, N STS, except that for three space-time streams, four training symbols
    // are required.
    return numberOfSpaceTimeStreams == 3 ? 4 : numberOfSpaceTimeStreams;
}

const simtime_t Ieee80211HTPreambleMode::getDuration() const
{
    // 20.3.7 Mathematical description of signals
    simtime_t sumOfHTLTFs = getFirstHTLongTrainingFieldDuration() + getSecondAndSubsequentHTLongTrainingFielDuration() * (numberOfHTLongTrainings - 1);
    if (preambleFormat == HT_PREAMBLE_MIXED)
        // L-STF -> L-LTF -> L-SIG -> HT-SIG -> HT-STF -> HT-LTF1 -> HT-LTF2 -> ... -> HT_LTFn
        return getNonHTShortTrainingSequenceDuration() + getNonHTLongTrainingFieldDuration() + legacySignalMode->getDuration() + highThroughputSignalMode->getDuration() + getHTShortTrainingFieldDuration() + sumOfHTLTFs;
    else if (preambleFormat == HT_PREAMBLE_GREENFIELD)
        // HT-GF-STF -> HT-LTF1 -> HT-SIG -> HT-LTF2 -> ... -> HT-LTFn
        return getHTGreenfieldShortTrainingFieldDuration() + highThroughputSignalMode->getDuration() + sumOfHTLTFs;
    else
        throw cRuntimeError("Unknown preamble format");
}

int Ieee80211HTModeBase::getNumberOfDataSubcarriers() const
{
    if (bandwidth == MHz(20))
        return 52;
    else if (bandwidth == MHz(40))
        // It is a special case, see Table 20-38—MCS parameters for
        // optional 40 MHz MCS 32 format, N SS = 1, N ES = 1
        return mcsIndex == 32 ? 48 : 108;
    else
        throw cRuntimeError("Unsupported bandwidth");
}

int Ieee80211HTModeBase::getNumberOfPilotSubcarriers() const
{
    if (bandwidth == MHz(20))
        return 4;
    else if (bandwidth == MHz(40))
        // It is a spacial case, see the comment above.
        return mcsIndex == 32 ? 4 : 6;
    else
        throw cRuntimeError("Unsupported bandwidth");
}

const simtime_t Ieee80211HTPreambleMode::getFirstHTLongTrainingFieldDuration() const
{
    if (preambleFormat == HT_PREAMBLE_MIXED)
        return simtime_t(4E-6);
    else if (preambleFormat == HT_PREAMBLE_GREENFIELD)
        return simtime_t(8E-6);
    else
        throw cRuntimeError("Unknown preamble format");
}

int Ieee80211HTDataMode::getBitLength(int dataBitLength) const
{
    return getServiceBitLength() + getTailBitLength() + dataBitLength; // TODO: padding?
}

unsigned int Ieee80211HTDataMode::computeNumberOfSpatialStreams(const Ieee80211OFDMModulation* stream1Modulation, const Ieee80211OFDMModulation* stream2Modulation, const Ieee80211OFDMModulation* stream3Modulation, const Ieee80211OFDMModulation* stream4Modulation) const
{
    return stream1Modulation ? 1 : 0 + stream2Modulation ? 1 : 0 +
           stream3Modulation ? 1 : 0 + stream4Modulation ? 1 : 0;
}

unsigned int Ieee80211HTDataMode::computeNumberOfCodedBitsPerSubcarrierSum() const
{
    return
        modulationAndCodingScheme->getModulation() ? modulationAndCodingScheme->getModulation()->getSubcarrierModulation()->getCodeWordSize() : 0 +
        modulationAndCodingScheme->getStreamExtension1Modulation() ? modulationAndCodingScheme->getStreamExtension1Modulation()->getSubcarrierModulation()->getCodeWordSize() : 0 +
        modulationAndCodingScheme->getStreamExtension2Modulation()? modulationAndCodingScheme->getStreamExtension2Modulation()->getSubcarrierModulation()->getCodeWordSize() : 0 +
        modulationAndCodingScheme->getStreamExtension3Modulation() ? modulationAndCodingScheme->getStreamExtension3Modulation()->getSubcarrierModulation()->getCodeWordSize() : 0;
}

unsigned int Ieee80211HTDataMode::computeNumberOfBccEncoders() const
{
    // When the BCC FEC encoder is used, a single encoder is used, except that two encoders
    // are used when the selected MCS has a PHY rate greater than 300 Mb/s (see 20.6).
    return getGrossBitrate() > Mbps(300) ? 2 : 1;
}

const simtime_t Ieee80211HTDataMode::getDuration(int dataBitLength) const
{
    unsigned int numberOfCodedBitsPerSubcarrierSum = computeNumberOfCodedBitsPerSubcarrierSum();
    unsigned int numberOfCodedBitsPerSymbol = numberOfCodedBitsPerSubcarrierSum * getNumberOfDataSubcarriers();
    const IForwardErrorCorrection *forwardErrorCorrection = getCode() ? getCode()->getForwardErrorCorrection() : nullptr;
    unsigned int dataBitsPerSymbol = forwardErrorCorrection ? forwardErrorCorrection->getDecodedLength(numberOfCodedBitsPerSymbol) : numberOfCodedBitsPerSymbol;
    int numberOfSymbols = lrint(ceil((double)getBitLength(dataBitLength) / dataBitsPerSymbol)); // TODO: getBitLength(dataBitLength) should be divisible by dataBitsPerSymbol
    return numberOfSymbols * getSymbolInterval();
}

const simtime_t Ieee80211HTMode::getSlotTime() const
{
    if (carrierFrequency == Hz(2400))
        return 20E-6;
    else if (carrierFrequency == Hz(5000))
        return 9E-6;
    else
        throw cRuntimeError("Unsupported carrier frequency");
}

inline const simtime_t Ieee80211HTMode::getSifsTime() const
{
    // 2.4: 10, 5: 16
    if (carrierFrequency == Hz(2400))
        return 10E-6;
    else if (carrierFrequency == Hz(5000))
        return 16E-6;
    else
        throw cRuntimeError("Sifs time is not defined for %f Hz carrier frequency", carrierFrequency.get());
}

const simtime_t Ieee80211HTMode::getShortSlotTime() const
{
    if (carrierFrequency == Hz(2400))
        return 9E-6;
    else
        throw cRuntimeError("Short slot time is not defined for %f Hz carrier frequency", carrierFrequency.get());
}


Ieee80211HTMCS::~Ieee80211HTMCS()
{
    delete code;
}

Ieee80211HTSignalMode::~Ieee80211HTSignalMode()
{
    delete code;
}

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs0BW20MHz(0, &Ieee80211OFDMCompliantModulations::bpskModulation, &Ieee80211OFDMCompliantCodes::ofdmConvolutionalCode1_2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs1BW20MHz(1, &Ieee80211OFDMCompliantModulations::qpskModulation, &Ieee80211OFDMCompliantCodes::ofdmConvolutionalCode1_2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs2BW20MHz(2, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs3BW20MHz(3, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs4BW20MHz(4, 16qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs5BW20MHz(5, 64qam, 2/3, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs6BW20MHz(6, 64qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs7BW20MHz(7, 64qam, 5/6, MHz(20));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs8BW20MHz(8, bpsk, bpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs9BW20MHz(9, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs10BW20MHz(10, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs11BW20MHz(11, 16qam, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs12BW20MHz(12, 16qam, 16qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs13BW20MHz(13, 64qam, 16qam, 2/3, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs14BW20MHz(14, 64qam, 64qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs15BW20MHz(15, 64qam, 64qam, 5/6, MHz(20));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs16BW20MHz(16, bpsk, bpsk, bpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs17BW20MHz(17, qpsk, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs18BW20MHz(18, qpsk, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs19BW20MHz(19, 16qam, 16qam, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs20BW20MHz(20, 16qam, 16qam, 16qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs21BW20MHz(21, 64qam, 16qam, 64qam, 2/3, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs22BW20MHz(22, 64qam, 64qam, 64qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs23BW20MHz(23, 64qam, 64qam, 64qam, 5/6, MHz(20));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs24BW20MHz(24, bpsk, bpsk, bpsk, bpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs25BW20MHz(25, qpsk, qpsk, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs26BW20MHz(26, qpsk, qpsk, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs27BW20MHz(27, 16qam, 16qam, 16qam, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs28BW20MHz(28, 16qam, 16qam, 16qam, 16qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs29BW20MHz(29, 64qam, 16qam, 64qam, 64qam, 2/3, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs30BW20MHz(30, 64qam, 64qam, 64qam, 64qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs31BW20MHz(31, 64qam, 64qam, 64qam, 64qam, 5/6, MHz(20));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs0BW20MHz(0, bpsk, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs1BW20MHz(1, bpsk, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs2BW20MHz(2, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs3BW20MHz(3, 16qam, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs4BW20MHz(4, 16qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs5BW20MHz(5, 64qam, 2/3, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs6BW20MHz(6, 64qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs7BW20MHz(7, 64qam, 5/6, MHz(40));


const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs8BW40MHz(8, bpsk, bpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs9BW40MHz(9, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs10BW40MHz(10, qpsk, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs11BW40MHz(11, 16qam, 16qam, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs12BW40MHz(12, 16qam, 16qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs13BW40MHz(13, 64qam, 16qam, 2/3, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs14BW40MHz(14, 64qam, 64qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs15BW40MHz(15, 64qam, 64qam, 5/6, MHz(40));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs16BW40MHz(16, bpsk, bpsk, bpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs17BW40MHz(17, qpsk, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs18BW40MHz(18, qpsk, qpsk, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs19BW40MHz(19, 16qam, 16qam, 16qam, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs20BW40MHz(20, 16qam, 16qam, 16qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs21BW40MHz(21, 64qam, 16qam, 64qam, 2/3, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs22BW40MHz(22, 64qam, 64qam, 64qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs23BW40MHz(23, 64qam, 64qam, 64qam, 5/6, MHz(40));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs24BW40MHz(24, bpsk, bpsk, bpsk, bpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs25BW40MHz(25, qpsk, qpsk, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs26BW40MHz(26, qpsk, qpsk, qpsk, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs27BW40MHz(27, 16qam, 16qam, 16qam, 16qam, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs28BW40MHz(28, 16qam, 16qam, 16qam, 16qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs29BW40MHz(29, 64qam, 16qam, 64qam, 64qam, 2/3, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs30BW40MHz(30, 64qam, 64qam, 64qam, 64qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs31BW40MHz(31, 64qam, 64qam, 64qam, 64qam, 5/6, MHz(40));

//const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs32BW20MHz(24, bpsk, bpsk, bpsk, bpsk, 1/2, MHz(40)); FIXME

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs33BW20MHz(33, 16qam, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs34BW20MHz(34, 64qam, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs35BW20MHz(35, 64qam, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs36BW20MHz(36, 16qam, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs37BW20MHz(37, 64qam, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs38BW20MHz(38, 64qam, 16qam, 3/4, MHz(20));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs39BW20MHz(39, 16qam, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs40BW20MHz(40, 16qam, 16qam, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs41BW20MHz(41, 64qam, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs42BW20MHz(42, 64qam, 16qam, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs43BW20MHz(43, 64qam, 16qam, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs44BW20MHz(44, 64qam, 64qam, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs45BW20MHz(45, 64qam, 64qam, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs46BW20MHz(46, 16qam, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs47BW20MHz(47, 16qam, 16qam, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs48BW20MHz(48, 64qam, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs49BW20MHz(49, 64qam, 16qam, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs50BW20MHz(50, 64qam, 16qam, 16qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs51BW20MHz(51, 64qam, 64qam, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs52BW20MHz(52, 64qam, 64qam, 16qam, 3/4, MHz(20));


const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs53BW20MHz(53, 16qam, qpsk, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs54BW20MHz(54, 16qam, 16qam, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs55BW20MHz(55, 16qam, 16qam, 16qam, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs56BW20MHz(56, 64qam, qpsk, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs57BW20MHz(57, 64qam, 16qam, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs58BW20MHz(58, 64qam, 16qam, 16qam, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs59BW20MHz(59, 64qam, 16qam, 16qam, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs60BW20MHz(60, 64qam, 64qam, qpsk, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs61BW20MHz(61, 64qam, 64qam, 16qam, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs62BW20MHz(62, 64qam, 64qam, 16qam, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs63BW20MHz(63, 64qam, 64qam, 64qam, qpsk, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs64BW20MHz(64, 64qam, 64qam, 64qam, 16qam, 1/2, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs65BW20MHz(65, 16qam, qpsk, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs66BW20MHz(66, 16qam, 16qam, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs67BW20MHz(67, 16qam, 16qam, 16qam, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs68BW20MHz(68, 64qam, qpsk, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs69BW20MHz(69, 64qam, 16qam, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs70BW20MHz(70, 64qam, 16qam, 16qam, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs71BW20MHz(71, 64qam, 16qam, 16qam, 16qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs72BW20MHz(72, 64qam, 64qam, qpsk, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs73BW20MHz(73, 64qam, 64qam, 16qam, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs74BW20MHz(74, 64qam, 64qam, 16qam, 16qam, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs75BW20MHz(75, 64qam, 64qam, 64qam, qpsk, 3/4, MHz(20));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs76BW20MHz(76, 64qam, 64qam, 64qam, qpsk, 3/4, MHz(20));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs33BW40MHz(33, 16qam, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs34BW40MHz(34, 64qam, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs35BW40MHz(35, 64qam, 16qam, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs36BW40MHz(36, 16qam, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs37BW40MHz(37, 64qam, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs38BW40MHz(38, 64qam, 16qam, 3/4, MHz(40));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs39BW40MHz(39, 16qam, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs40BW40MHz(40, 16qam, 16qam, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs41BW40MHz(41, 64qam, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs42BW40MHz(42, 64qam, 16qam, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs43BW40MHz(43, 64qam, 16qam, 16qam, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs44BW40MHz(44, 64qam, 64qam, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs45BW40MHz(45, 64qam, 64qam, 16qam, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs46BW40MHz(46, 16qam, qpsk, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs47BW40MHz(47, 16qam, 16qam, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs48BW40MHz(48, 64qam, qpsk, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs49BW40MHz(49, 64qam, 16qam, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs50BW40MHz(50, 64qam, 16qam, 16qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs51BW40MHz(51, 64qam, 64qam, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs52BW40MHz(52, 64qam, 64qam, 16qam, 3/4, MHz(40));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs53BW40MHz(53, 16qam, qpsk, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs54BW40MHz(54, 16qam, 16qam, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs55BW40MHz(55, 16qam, 16qam, 16qam, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs56BW40MHz(56, 64qam, qpsk, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs57BW40MHz(57, 64qam, 16qam, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs58BW40MHz(58, 64qam, 16qam, 16qam, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs59BW40MHz(59, 64qam, 16qam, 16qam, 16qam, 1/2, MHz(40));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs60BW40MHz(60, 64qam, 64qam, qpsk, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs61BW40MHz(61, 64qam, 64qam, 16qam, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs62BW40MHz(62, 64qam, 64qam, 16qam, 16qam, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs63BW40MHz(63, 64qam, 64qam, 64qam, qpsk, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs64BW40MHz(64, 64qam, 64qam, 64qam, 16qam, 1/2, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs65BW40MHz(65, 16qam, qpsk, qpsk, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs66BW40MHz(66, 16qam, 16qam, qpsk, qpsk, 3/4, MHz(40));

const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs67BW40MHz(67, 16qam, 16qam, 16qam, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs68BW40MHz(68, 64qam, qpsk, qpsk, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs69BW40MHz(69, 64qam, 16qam, qpsk, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs70BW40MHz(70, 64qam, 16qam, 16qam, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs71BW40MHz(71, 64qam, 16qam, 16qam, 16qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs72BW40MHz(72, 64qam, 64qam, qpsk, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs73BW40MHz(73, 64qam, 64qam, 64qam, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs74BW40MHz(74, 64qam, 64qam, 16qam, 16qam, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs75BW40MHz(75, 64qam, 64qam, 64qam, qpsk, 3/4, MHz(40));
const Ieee80211HTMCS Ieee80211HTMCSTable::htMcs76BW40MHz(76, 64qam, 64qam, 64qam, 16qam, 3/4, MHz(40));

} /* namespace physicallayer */
} /* namespace inet */
