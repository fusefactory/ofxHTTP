// =============================================================================
//
// Copyright (c) 2013 Christopher Baker <http://christopherbaker.net>
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


#include "ofEvents.h"
#include "ofFileUtils.h"
#include "ofx/HTTP/Client/BaseRequest.h"
#include "ofx/HTTP/Client/BaseResponse.h"
#include "ofx/HTTP/Client/Context.h"


namespace ofx {
namespace HTTP {
namespace Client {


class BaseClientRequestArgs: public ofEventArgs
{
public:
    BaseClientRequestArgs(const BaseRequest& request,
                          Context& context):
        _request(request),
        _context(context)
    {
    }


    virtual ~BaseClientRequestArgs()
    {
    }


    const BaseRequest& getRequest() const
    {
        return _request;
    }

    Context& getContextRef()
    {
        return _context;
    }

protected:
    const BaseRequest& _request;
    Context& _context;
    
};


class BaseProgressArgs
{
public:
    BaseProgressArgs(std::streamsize bytesTransferred):
        _bytesTransferred(bytesTransferred)
    {
    }

    virtual ~BaseProgressArgs()
    {
    }

    virtual std::streamsize getContentLength() const = 0;


    /// \brief Get the total number of bytes transferred during this request.
    /// \returns the total number of bytes transferred.
    std::streamsize getBytesTransferred() const
    {
        return _bytesTransferred;
    }

    /// \brief Get the progress of the request upload.
    ///
    /// If the total content length is unknown (e.g. during chunked-transfer)
    /// progress of Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH will be
    /// returned.
    ///
    /// \returns a value 0.0 - 1.0 iff the content length is known.  Otherwise
    /// returns Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH.
    float getProgress() const
    {
        std::streamsize contentLength = getContentLength();

        if (Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH == contentLength)
        {
            return Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH;
        }
        else
        {
            return _bytesTransferred / (double)contentLength;
        }
    }

protected:
    std::streamsize _bytesTransferred;

};


class ClientRequestProgressArgs:
    public BaseClientRequestArgs,
    public BaseProgressArgs
{
public:
    ClientRequestProgressArgs(const BaseRequest& request,
                              Context& context,
                              std::streamsize bytesTransferred):
        BaseClientRequestArgs(request, context),
        BaseProgressArgs(bytesTransferred)
    {
    }

    virtual ~ClientRequestProgressArgs()
    {
    }

    /// \brief Get the total content length associated with this request.
    ///
    /// If the total content length is unknown (e.g. during chunked-transfer)
    /// Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH is returned.
    ///
    /// \returns the total length of the content in bytes iff the content length
    /// is known.  Otherwise returns
    /// Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH.
    std::streamsize getContentLength() const
    {
        return _request.getContentLength();
    }
};


class BaseClientResponseArgs: public BaseClientRequestArgs
{
public:
    BaseClientResponseArgs(const BaseRequest& request,
                           const BaseResponse& response,
                           Context& context):
        BaseClientRequestArgs(request, context),
        _response(response)
    {
    }

    
    virtual ~BaseClientResponseArgs()
    {
    }


    const BaseResponse& getResponse() const
    {
        return _response;
    }

protected:
    const BaseResponse& _response;

};


class ClientResponseProgressArgs:
    public BaseClientResponseArgs,
    public BaseProgressArgs
{
public:
    ClientResponseProgressArgs(const BaseRequest& request,
                               const BaseResponse& response,
                               Context& context,
                               std::streamsize bytesTransferred):
        BaseClientResponseArgs(request, response, context),
        BaseProgressArgs(bytesTransferred)
    {
    }

    virtual ~ClientResponseProgressArgs()
    {
    }

    /// \brief Get the total content length associated with this request.
    ///
    /// If the total content length is unknown (e.g. during chunked-transfer)
    /// Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH is returned.
    ///
    /// \returns the total length of the content in bytes iff the content length
    /// is known.  Otherwise returns
    /// Poco::Net::HTTPMessage::UNKNOWN_CONTENT_LENGTH.
    std::streamsize getContentLength() const
    {
        return _response.getContentLength();
    }
};



class ClientResponseEventArgs: public BaseClientResponseArgs
{
public:
    ClientResponseEventArgs(std::istream& responseStream,
                            const BaseRequest& request,
                            const BaseResponse& response,
                            Context& context):
        BaseClientResponseArgs(request, response, context),
        _responseStream(responseStream)
    {
    }


    virtual ~ClientResponseEventArgs()
    {
    }

    std::istream& getResponseStream()
    {
        return _responseStream;
    }

protected:
    std::istream& _responseStream;
    
};


class ClientErrorEventArgs: public BaseClientResponseArgs
{
public:
    ClientErrorEventArgs(const BaseRequest& request,
                         const BaseResponse& response,
                         Context& context,
                         const Poco::Exception& exception):
        BaseClientResponseArgs(request, response, context),
        _exception(exception)
    {
    }


    virtual ~ClientErrorEventArgs()
    {
    }


    const Poco::Exception& getException() const
    {
        return _exception;
    }

protected:
    const Poco::Exception& _exception;

};


class ClientEvents
{
public:
    ofEvent<BaseClientRequestArgs>   onHTTPClientRequestFilterEvent;
    ofEvent<BaseClientResponseArgs>  onHTTPClientResponseFilterEvent;

    ofEvent<ClientErrorEventArgs>    onHTTPClientErrorEvent;
    ofEvent<ClientResponseEventArgs> onHTTPClientResponseEvent;

    ofEvent<ClientRequestProgressArgs> onHTTPClientRequestProgress;
    ofEvent<ClientResponseProgressArgs> onHTTPClientResponseProgress;

};


} } } // namespace ofx::HTTP::Client
