#pragma once

#define USE_BOOST_UNITS 1

#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_access.hpp>
#include <boost/qvm/vec_operations.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdint>
#include <istream>
#include <ostream>

namespace bitflow::model
{

// Math
using Vector3f = boost::qvm::vec<float, 3>;
using Vector2f = boost::qvm::vec<float, 2>;

} // namespace bitflow::model

//inline std::ostream& operator<<(std::ostream& out, bitflow::model::Vector3f v)
//{
//  return out << "(" << v.a[0] << "," << v.a[1] << "," << v.a[2] << ")";
//}

#if USE_BOOST_UNITS

#include "boost/units/quantity.hpp"
#include "boost/units/systems/si/length.hpp"
#include "boost/units/systems/si/time.hpp"
#include "boost/units/systems/si/velocity.hpp"
#include "boost/units/systems/information/bit.hpp"
#include "boost/units/static_constant.hpp"
#include "boost/units/make_scaled_unit.hpp"
#include "boost/units/static_rational.hpp"

namespace bitflow::model
{
// Distance / size / position
using boost::units::si::length;
using boost::units::si::meter;

using Length = boost::units::quantity<length, float>;
using Position = boost::qvm::vec<Length, 2>;
using Direction = boost::qvm::vec<float, 2>;

// Velocity
using boost::units::si::velocity;
using boost::units::si::meters_per_second;

using Velocity = boost::units::quantity<velocity, float>;

// Time
using boost::units::si::time;
using boost::units::si::second;
using millisecond_unit = boost::units::make_scaled_unit < time, boost::units::scale<10, boost::units::static_rational<-3>> >::type;
using microsecond_unit = boost::units::make_scaled_unit < time, boost::units::scale<10, boost::units::static_rational<-6>> >::type;
using nanosecond_unit = boost::units::make_scaled_unit<time, boost::units::scale<10, boost::units::static_rational<-9>> >::type;
BOOST_UNITS_STATIC_CONSTANT(millisecond, millisecond_unit);
BOOST_UNITS_STATIC_CONSTANT(microsecond, microsecond_unit);
BOOST_UNITS_STATIC_CONSTANT(nanosecond, nanosecond_unit);

using Time = boost::units::quantity<time, float>;

// Information
using info = boost::units::information::info;
using boost::units::information::byte;
using kilobytes_unit = boost::units::make_scaled_unit < info, boost::units::scale<10, boost::units::static_rational<3>> >::type;
using megabytes_unit = boost::units::make_scaled_unit < info, boost::units::scale<10, boost::units::static_rational<6>> >::type;
using gigabytes_unit = boost::units::make_scaled_unit<info, boost::units::scale<10, boost::units::static_rational<9>> >::type;
BOOST_UNITS_STATIC_CONSTANT(kilobytes, kilobytes_unit);
BOOST_UNITS_STATIC_CONSTANT(megabytes, megabytes_unit);
BOOST_UNITS_STATIC_CONSTANT(gigabytes, gigabytes_unit);

using Info = boost::units::quantity<info, float>;

// Information rate
using bandwidth_dimension = boost::units::derived_dimension<boost::units::information_base_dimension, 1, boost::units::time_base_dimension, -1>::type;
using bandwidth_unit = boost::units::unit < bandwidth_dimension, boost::units::information::system >;
BOOST_UNITS_STATIC_CONSTANT(bytes_per_second, bandwidth_unit);
using kilobytes_per_second_unit = boost::units::make_scaled_unit < bandwidth_unit, boost::units::scale<10, boost::units::static_rational<3>> >::type;
using megabytes_per_second_unit = boost::units::make_scaled_unit < bandwidth_unit, boost::units::scale<10, boost::units::static_rational<6>> >::type;
using gigabytes_per_second_unit = boost::units::make_scaled_unit<bandwidth_unit, boost::units::scale<10, boost::units::static_rational<9>> >::type;
BOOST_UNITS_STATIC_CONSTANT(kilobytes_per_second, kilobytes_per_second_unit);
BOOST_UNITS_STATIC_CONSTANT(megabytes_per_second, megabytes_per_second_unit);
BOOST_UNITS_STATIC_CONSTANT(gigabytes_per_second, gigabytes_per_second_unit);
using Bandwidth = boost::units::quantity<bandwidth_unit, float>;

} // namespace bitflow::model

inline bitflow::model::Vector2f value(bitflow::model::Position const& position)
{
  return bitflow::model::Vector2f{ X(position).value(), Y(position).value() };
}

inline bitflow::model::Position operator*(bitflow::model::Direction const& direction, bitflow::model::Length const length)
{
  return bitflow::model::Position{ { boost::qvm::X(direction) * length, boost::qvm::Y(direction) * length } };
}

inline bitflow::model::Position operator*(bitflow::model::Position const& position, float scalar)
{
  return bitflow::model::Position{ { boost::qvm::X(position) * scalar, boost::qvm::Y(position) * scalar } };
}

inline bitflow::model::Length mag(bitflow::model::Position const& position)
{
  return mag(bitflow::model::Vector2f{ X(position).value(), Y(position).value() }) * bitflow::model::meter;
}

inline bitflow::model::Direction operator/(bitflow::model::Position const& position, bitflow::model::Length length)
{
  return bitflow::model::Direction{ { boost::qvm::X(position) / length, boost::qvm::Y(position) / length } };
}
// Units stream operator overloads
namespace boost::units {

inline std::ostream& operator<<(std::ostream& out, bitflow::model::Time const& time)
{
  return out << time.value();
}

inline std::istream& operator>>(std::istream& in, bitflow::model::Time& time)
{
  typename bitflow::model::Length::value_type value;
  in >> value;
  time = value * bitflow::model::second;

  return in;
}

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

inline std::ostream& operator<<(std::ostream& out, bitflow::model::Velocity const& velocity)
{
  return out << velocity.value();
}

inline std::istream& operator>>(std::istream& in, bitflow::model::Velocity& velocity)
{
  typename bitflow::model::Velocity::value_type value;
  in >> value;
  velocity = value * bitflow::model::meters_per_second;

  return in;
}

inline std::ostream& operator<<(std::ostream& out, bitflow::model::Info const& infoAmount)
{
  return out << infoAmount.value();
}
//

inline std::istream& operator>>(std::istream& in, bitflow::model::Info& infoAmount)
{
  float value;
  in >> value;
  infoAmount = value * bitflow::model::byte;

  return in;
}

inline std::ostream& operator<<(std::ostream& out, bitflow::model::Bandwidth const& bandwidth)
{
  return out << boost::units::quantity_cast<float>(boost::units::quantity<bitflow::model::megabytes_per_second_unit, float>(bandwidth));
}
//

inline std::istream& operator>>(std::istream& in, bitflow::model::Bandwidth& bandwidth)
{
  float value;
  in >> value;
  bandwidth = bitflow::model::Bandwidth(value * bitflow::model::megabytes_per_second);

  return in;
}

} // namespace boost::units

#else

namespace bitflow::model
{
// Distance / size / position
using Length = float;
using Position = Vector3f;

Length const meter = 1.0f;

// Time
using Time = float;

Time const second = 1.0f;

// Information
using Info = std::uint32_t;

Info const bit = 1;
Info const byte = 8;

using Bandwidth = float;

} // namespace bitflow::model

#endif

namespace boost::qvm
{

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

} // namespace boost::qvm
