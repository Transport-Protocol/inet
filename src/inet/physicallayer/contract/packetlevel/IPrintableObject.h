//
// Copyright (C) 2013 OpenSim Ltd.
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

#ifndef __INET_IPRINTABLEOBJECT_H
#define __INET_IPRINTABLEOBJECT_H

#include "inet/physicallayer/base/packetlevel/PhysicalLayerDefs.h"

namespace inet {

namespace physicallayer {

/**
 * This purely virtual interface provides an abstraction for printable objects.
 */
class INET_API IPrintableObject
{
  public:
    enum PrintLevel {
        PRINT_LEVEL_INFO,
        PRINT_LEVEL_DETAIL,
        PRINT_LEVEL_DEBUG,
        PRINT_LEVEL_TRACE
    };

  public:
    virtual ~IPrintableObject() {}

    /**
     * Prints this object to the provided output stream.
     *
     * Function calls to operator<< with pointers or references either const
     * or not are all forwarded to this function.
     */
    virtual void printToStream(std::ostream& stream, int level) const = 0;

    virtual std::string getInfoString() const { std::stringstream s; printToStream(s, PRINT_LEVEL_INFO); return s.str(); }

    virtual std::string getDetailString() const { std::stringstream s; printToStream(s, PRINT_LEVEL_DETAIL); return s.str(); }

    virtual std::string getDebugString() const { std::stringstream s; printToStream(s, PRINT_LEVEL_DEBUG); return s.str(); }

    virtual std::string getTraceString() const { std::stringstream s; printToStream(s, PRINT_LEVEL_TRACE); return s.str(); }
};

inline std::ostream& operator<<(std::ostream& stream, IPrintableObject *object)
{
    object->printToStream(stream, IPrintableObject::PRINT_LEVEL_DETAIL);
    return stream;
};

inline std::ostream& operator<<(std::ostream& stream, IPrintableObject& object)
{
    object.printToStream(stream, IPrintableObject::PRINT_LEVEL_DETAIL);
    return stream;
};

inline std::ostream& operator<<(std::ostream& stream, const IPrintableObject *object)
{
    object->printToStream(stream, IPrintableObject::PRINT_LEVEL_DETAIL);
    return stream;
};

inline std::ostream& operator<<(std::ostream& stream, const IPrintableObject& object)
{
    object.printToStream(stream, IPrintableObject::PRINT_LEVEL_DETAIL);
    return stream;
};

} // namespace physicallayer

} // namespace inet

#endif // ifndef __INET_IPRINTABLEOBJECT_H

