// NOTE: adapted from http://stackoverflow.com/questions/26380420/boost-fusion-sequence-type-and-name-identification-for-structs-and-class
#pragma once

#include <boost/optional.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <any>

namespace visitor {

  template <typename Flavour, typename T> struct VisitorApplication;

  namespace detail
  {
    template <typename V, typename Enable = void>
    struct is_vector : boost::mpl::false_ { };

    template <typename T>
    struct is_vector<std::vector<T>, void> : boost::mpl::true_ { };

    namespace iteration
    {
      // Iteration over a sequence
      template <typename FusionVisitorConcept, typename S, typename N>
      struct members_impl
      {
        // Type of the current member
        typedef typename boost::fusion::result_of::value_at<S, N>::type   current_t;
        typedef typename boost::mpl::next<N>::type                        next_t;
        typedef boost::fusion::extension::struct_member_name<S, N::value> name_t;

        static inline void handle(FusionVisitorConcept& visitor, S& s)
        {
          if (VisitorApplication<FusionVisitorConcept, current_t>::handle(visitor, name_t::call(), boost::fusion::at<N>(s)))
          {
            members_impl<FusionVisitorConcept, S, next_t>::handle(visitor, s);
          }
        }
      };

      // End condition of sequence iteration
      template <typename FusionVisitorConcept, typename S>
      struct members_impl<FusionVisitorConcept, S, typename boost::fusion::result_of::size<S>::type>
      {
        static inline void handle(FusionVisitorConcept const&, const S&) { /*Nothing to do*/ }
      };

      // Iterate over struct/sequence. Base template
      template <typename FusionVisitorConcept, typename S>
      struct Struct : members_impl<FusionVisitorConcept, S, boost::mpl::int_<0>> {};

    } // iteration

    template <typename FusionVisitorConcept, typename T>
    struct array_application
    {
      typedef array_application<FusionVisitorConcept, T> type;

      typedef typename T::value_type value_type;

      static inline void handle(FusionVisitorConcept& visitor, const T& t)
      {
        for (auto& el : t)
          VisitorApplication<FusionVisitorConcept, value_type>::handle(visitor, el);
      }
    };

    template <typename FusionVisitorConcept, typename T>
    struct struct_application
    {
      typedef struct_application<FusionVisitorConcept, T> type;

      static inline void handle(FusionVisitorConcept& visitor, T& t)
      {
        iteration::Struct<FusionVisitorConcept, T>::handle(visitor, t);
      }
    };

    template <typename FusionVisitorConcept, typename T, typename Enable = void>
    struct value_application
    {
      typedef value_application<FusionVisitorConcept, T> type;

      static inline void handle(FusionVisitorConcept& visitor, T& t) {
        visitor.value(t);
      }

      static inline bool handle(FusionVisitorConcept& visitor, const char* name, T& t) {
        return visitor.value(name, t);
      }
    };

    template <typename FusionVisitorConcept, typename T>
    struct value_application<FusionVisitorConcept, boost::optional<T> >
    {
      typedef value_application<FusionVisitorConcept, boost::optional<T> > type;

      static inline void handle(FusionVisitorConcept& visitor, const boost::optional<T>& t) {
        if (t)
          VisitorApplication<FusionVisitorConcept, T>::handle(visitor, *t);
        else
          ; // perhaps some default action?
      }
    };

    template <typename FusionVisitorConcept, typename T>
    struct select_application
    {
      typedef
        //typename boost::mpl::eval_if<boost::is_array<T>,                  boost::mpl::identity<array_application<FusionVisitorConcept, T>>,
        typename boost::mpl::eval_if<detail::is_vector<T>, boost::mpl::identity<array_application <FusionVisitorConcept, T>>,
        typename boost::mpl::eval_if<boost::fusion::traits::is_sequence<T>, boost::mpl::identity<struct_application<FusionVisitorConcept, T>>,
        boost::mpl::identity<value_application<FusionVisitorConcept, T>>
        > >::type type;
    };

  } // detail

  template <typename FusionVisitorConcept, typename T>
  struct VisitorApplication : public detail::select_application<FusionVisitorConcept, T>::type
  {
  };
}

template <typename FusionVisitorConcept, typename T>
void apply_fusion_visitor(FusionVisitorConcept& visitor, T& o)
{
  visitor::VisitorApplication<FusionVisitorConcept, T>::handle(visitor, o);
}

struct DisplayMemberVisitor 
{
    DisplayMemberVisitor() { ss << std::boolalpha; }

    std::string complete() { return ss.str(); }

    template <typename T> bool value(const char* name, T const& value)
    {
        ss << name << "=" << value << std::endl;
        return true;
    }

private:
    std::stringstream ss;
};

template <typename T>
struct SetMemberValueFromStrVisitor
{
  SetMemberValueFromStrVisitor(std::string_view const& _memberName, std::string _value) : memberName(_memberName), value_to_set(_value)
  {}

  template <typename T> bool value(const char* _name, T& value_) 
  {
    if (memberName == _name)
    {
      value_ = boost::lexical_cast<T>(value_to_set);
      return false;
    }

    return true;
  }

  std::string_view memberName;
  std::string value_to_set;
};