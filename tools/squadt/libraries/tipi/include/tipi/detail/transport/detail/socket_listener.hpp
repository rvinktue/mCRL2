// Author(s): Jeroen van der Wulp
// Copyright: see the accompanying file COPYING or copy at
// https://svn.win.tue.nl/trac/MCRL2/browser/trunk/COPYING
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
/// \file transport/detail/socket_listener.hpp

#ifndef SOCKET_LISTENER_H
#define SOCKET_LISTENER_H

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <tipi/detail/transport/detail/socket_transceiver.hpp>
#include <tipi/detail/transport/detail/listener.ipp>

namespace transport {
  /// \internal
  namespace listener {

    /// \internal
    class socket_listener : public basic_listener {

      private:

        boost::shared_ptr< socket_scheduler > scheduler;

        /** \brief The socket listener */
        boost::asio::ip::tcp::acceptor        acceptor;

        /** \brief For mutual exclusive event handling */
        boost::asio::strand                   dispatcher;

      private:

        /** \brief Handler for incoming socket connections */
        void handle_accept(const boost::system::error_code&,
                boost::shared_ptr< transceiver::socket_transceiver >,
                boost::shared_ptr< basic_listener >);

      public:

        /** \brief Constructor */
        socket_listener(boost::shared_ptr < transport::transporter_impl > const&, boost::asio::ip::address const&, short int const& = 0);

        /** \brief Activate the listener */
        void activate(boost::shared_ptr< basic_listener >);

        /** \brief Schedule shutdown of listener */
        void shutdown() {
          acceptor.close();
        }

        /** \brief Destructor */
        virtual ~socket_listener() {
          shutdown();
        }
    };
  }
}

#endif
