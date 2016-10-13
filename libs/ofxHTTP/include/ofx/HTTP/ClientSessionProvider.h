// =============================================================================
//
// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include "ofx/HTTP/AbstractClientTypes.h"
#include "ofx/HTTP/HTTPHost.h"


namespace ofx {
namespace HTTP {


//class ClientSession: public AbstractClientSession
//{
//public:
//    ClientSession(const HTTPHost& host);
//
//    virtual ~ClientSession();
//
//    virtual AbstractClientSessionProvider* parent() const;
//
//private:
//    ClientSessionProvider* _parent = nullptr;
//
//};



/// \brief Set up a client session based on the requested URI scheme and host.
class ClientSessionProvider: public AbstractClientSessionProvider
{
public:
    ClientSessionProvider();

    virtual ~ClientSessionProvider();

    virtual void requestFilter(Context& context,
                               BaseRequest& request) const override;

    virtual void returnClientSession(std::unique_ptr<Poco::Net::HTTPClientSession> session) const override;

private:
    /// \brief Attempt to determine the host information from a request or context.
    static HTTPHost _extractHost(const Context& context,
                                 const BaseRequest& request);

    static HTTPHost _extractHost(const Poco::Net::HTTPClientSession& session);

    /// \brief Attempt to determine the can be used to contact the given host.
    static bool _isValidSessionForHost(const Poco::Net::HTTPClientSession& session,
                                       const HTTPHost& host);

    /// \brief Get session for the given host and context.
    void _getSessionForRequest(Context& context, const HTTPHost& host) const;

    /// \brief The session pool. It is effectively a cache, and thus mutable.
    mutable std::vector<std::unique_ptr<Poco::Net::HTTPClientSession>> _sessions;

    /// \brief The mutex for multi-threaded access.
    mutable std::mutex _mutex;

};


} } // namespace ofx::HTTP
