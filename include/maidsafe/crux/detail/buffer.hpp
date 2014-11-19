///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2014 MaidSafe.net Limited
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MAIDSAFE_CRUX_DETAIL_BUFFER_HPP
#define MAIDSAFE_CRUX_DETAIL_BUFFER_HPP

#include <algorithm>
#include <vector>

namespace maidsafe
{
namespace crux
{
namespace detail
{

using buffer = std::vector<char>;

class buffers {
  public:
    typedef boost::asio::mutable_buffer value_type;

    struct const_iterator {
      public:
        const value_type& operator++ ();   // prefix
        const value_type operator++ (int); // postfix
        const value_type& operator*() const;

        const_iterator();
        const_iterator(value_type& header, std::vector<value_type>& payload);

      private:
        size_t                   i;
        value_type*              header;
        std::vector<value_type>* payload;
    };

    const_iterator begin() const;
    const_iterator end()   const;

    buffers() {}

    template<class MutableBufferSequence>
    buffers(const value_type& header, const MutableBufferSequence& payload);

  private:
    value_type              header;
    std::vector<value_type> payload;
};

} // namespace detail
} // namespace crux
} // namespace maidsafe

#include <boost/asio/buffer.hpp>

namespace maidsafe { namespace crux { namespace detail {

template<class MutableBufferSequence>
buffers::buffers(const value_type& header, const MutableBufferSequence& payload)
  : header(header)
{
    for (auto i = payload.begin(); i != payload.end(); ++i) {
        this->payload.emplace_back(*i);
    }
}

inline
buffers::const_iterator::const_iterator()
    : i(0), header(nullptr), payload(nullptr) { }

inline
buffers::const_iterator::const_iterator(value_type& header,
                                        std::vector<value_type>& payload)
    : i(0), header(&header), payload(&payload) {} 

inline
const buffers::value_type& buffers::const_iterator::operator++() { // prefix
    return (*payload)[++i - 1];
}

inline
const buffers::value_type buffers::const_iterator::operator++(int) { // postfix
    if (i == 0) {
        ++i;
        return *header;
    }
    else {
        return (*payload)[i++ - 1];
    }
}

inline
const buffers::value_type& buffers::const_iterator::operator*() const {
    if (i == 0) {
        return *header;
    }
    else {
        return (*payload)[i - 1];
    }
}

} // namespace detail
} // namespace asio
} // namespace boost

#endif // MAIDSAFE_CRUX_DETAIL_BUFFER_HPP
