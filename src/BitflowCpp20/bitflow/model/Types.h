#pragma once

#define USE_BOOST_UNITS 1

#include <boost/qvm/vec.hpp>
#include <cstdint>
#include <ostream>

namespace bitflow
{

namespace model
{

// Math
using Vector3f = boost::qvm::vec<float, 3>;
using Vector2f = boost::qvm::vec<float, 2>;


} // namespace model
} // namespace bitflow

//inline std::ostream& operator<<(std::ostream& out, bitflow::model::Vector3f v)
//{
//  return out << "(" << v.a[0] << "," << v.a[1] << "," << v.a[2] << ")";
//}

#if USE_BOOST_UNITS

#include "boost/units/quantity.hpp"
#include "boost/units/systems/si/length.hpp"
#include "boost/units/systems/si/time.hpp"
#include "boost/units/systems/information/bit.hpp"
#include "boost/units/systems/information/byte.hpp"
#include "boost/mpl/divides.hpp"
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>
#include <boost/lexical_cast.hpp>
#include <ostream>
#include <istream>


namespace bitflow
{


namespace model
{

// Distance / size / position
using boost::units::si::length;
using Length = boost::units::quantity<length, float>;
using Position = boost::qvm::vec<Length, 2>;


using boost::units::si::meter;

// Time
using boost::units::si::time;
using Time = boost::units::quantity<time, float>;

// Information
using boost::units::information::info;
using InfoAmount = boost::units::quantity<info, std::uint32_t>;

using boost::units::information::bit;
using boost::units::information::byte;


using bandwidth = boost::mpl::divides<boost::units::information_dimension, boost::units::time_dimension>::type;
using Bandwidth = boost::units::quantity<bandwidth, float>;

} // namespace model

} // namespace bitflow

inline bitflow::model::Position operator*(bitflow::model::Position const& position, float scalar)
{
  return bitflow::model::Position{ { boost::qvm::X(position) * scalar, boost::qvm::Y(position) * scalar } };
}

// Units stream operator overloads
namespace boost { namespace units {

inline std::ostream& operator<<(std::ostream& out, bitflow::model::Length const& length)
{
  return out << length.value();
}

inline std::istream& operator>>(std::istream& in, bitflow::model::Length& length)
{
  typename bitflow::model::Length::value_type value;
  in >> value;
  length = value * bitflow::model::meter;

  return in;
}

inline std::ostream& operator<<(std::ostream& out, bitflow::model::InfoAmount const& infoAmount)
{
  return out << infoAmount.value() << " bytes";
}
//

inline std::istream& operator>>(std::istream& in, bitflow::model::InfoAmount& infoAmount)
{
  float value;
  in >> value;
  infoAmount = value * bitflow::model::byte;

  return in;
}

} } // namespace boost::units

namespace boost { namespace qvm {

inline std::ostream& operator<<(std::ostream& out, bitflow::model::Position const& position)
{
  return out << "(" << X(position) << "," << Y(position) << ")";
}

inline std::istream& operator>>(std::istream& in, bitflow::model::Position& position)
{
  char c;
  in >> c;
  assert(c == '(');

  for (int i = 0; i < 2; ++i)
  {
    float coordinate;
    in >> coordinate;
    position.a[i] = coordinate * bitflow::model::meter;

    in >> c;
    assert(c == (i < 1 ? ',' : ')'));
  }

  return in;
}

} } // namespace boost::qvm

#else

namespace bitflow
{
// Distance / size / position
typedef float Length;
typedef Vector3f Position;

Length const meter = 1.0f;

// Time
typedef float Time;

Time const second = 1.0f;

// Information
typedef std::uint32_t InfoAmount;

InfoAmount const bit = 1;
InfoAmount const byte = 8;

typedef float Bandwidth;

} // namespace bitflow

#endif

