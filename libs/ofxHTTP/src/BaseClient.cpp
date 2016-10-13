//// =============================================================================
////
//// Copyright (c) 2013-2016 Christopher Baker <http://christopherbaker.net>
////
//// Permission is hereby granted, free of charge, to any person obtaining a copy
//// of this software and associated documentation files (the "Software"), to deal
//// in the Software without restriction, including without limitation the rights
//// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//// copies of the Software, and to permit persons to whom the Software is
//// furnished to do so, subject to the following conditions:
////
//// The above copyright notice and this permission notice shall be included in
//// all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//// THE SOFTWARE.
////
//// =============================================================================
//
//
//#include "ofx/HTTP/BaseClient.h"
//
//
//namespace ofx {
//namespace HTTP {
//
//
//BaseClient::BaseClient()
//{
//}
//
//
//BaseClient::BaseClient(std::vector<AbstractRequestFilter*> requestFilters,
//                       std::vector<AbstractResponseFilter*> responseFilters):
//    _requestFilters(requestFilters),
//    _responseFilters(responseFilters)
//{
//}
//
//
//BaseClient::~BaseClient()
//{
//    // We do not own the filter pointers.
//}
//
//
//void BaseClient::execute(Context& context,
//                         BaseRequest& request,
//                         BaseResponse& response)
//{
//    try
//    {
//        // We set resubmit false because we are executing a new request.
//        context.setResubmit(false);
//
//        // Apply request filters.
//        for (auto& filter: _requestFilters)
//        {
//            filter->requestFilter(context, request);
//        }
//
//        // Prepare the request. Depending on the request type, this may involve
//        // signing a requests, counting Content-Bytes, etc.
//        request.prepareRequest();
//
//        context.setState(ClientState::SENDING_HEADERS);
//
//        if (context.clientSession() == nullptr)
//        {
//            throw Poco::Exception("No session available for request.");
//        }
//
//        context.setState(ClientState::SENDING_CONTENTS);
//
////        std::ostream& rawRequestStream = context.getClientSession()->sendRequest(request);
////
//////         Send the request headers and get the request stream.
////        ClientProgressRequestStream requestStream(context,
////                                                  request,
////                                                  rawRequestStream);
//
//        auto& requestStream = context.clientSession()->sendRequest(request);
//
//        // Write the request body to the output stream.
//        request.writeRequestBody(requestStream);
//
//        context.setState(ClientState::RECEIVING_HEADERS);
//
//        if (context.clientSession() == nullptr)
//        {
//            throw Poco::Exception("No session available for request.");
//        }
//
//        IO::FilteredInputStream filteredResponseStream(context.clientSession()->receiveResponse(response));
//
//        // Apply response filters. Response filters determine whether redirects
//        // will be needed and whether resubmits will be needed.
//        for (auto& filter: _responseFilters)
//        {
//            filter->responseFilter(context, request, response);
//        }
//
//        context.setState(ClientState::RECEIVING_CONTENTS);
//
//        // Apply response stream filters.
//        for (auto& filter: _responseStreamFilters)
//        {
//            filter->responseStreamFilter(context,
//                                         request,
//                                         response,
//                                         filteredResponseStream);
//        }
//
//        ClientProgressResponseStream responseStream(context,
//                                                    request,
//                                                    response,
//                                                    filteredResponseStream);
//
//        // Check to see if a response filter requested a resubmission.
//        if (context.getResubmit())
//        {
//            // If a response filter did not reset the session, then we must
//            // consume the stream in order to reuse the HTTP session.
//            if (context.clientSession() != nullptr)
//            {
//                // Consume the current response stream so we are ready to re-use
//                // the existing session.
//                HTTPUtils::consume(responseStream);
//            }
//
//            context.setState(ClientState::REDIRECTING);
//
//            // Re-submit the update request.
//            execute(context, request, response);
//        }
//        else
//        {
//            ClientResponseStreamEventArgs args(context,
//                                               request,
//                                               response);
//
//            ofNotifyEvent(context.events.onHTTPClientResponseStream, args, this);
//        }
//    }
//    catch (const Poco::SyntaxException& exc)
//    {
//        ClientErrorEventArgs args(context, request, response, exc);
//        ofNotifyEvent(context.events.onHTTPClientError, args, this);
//    }
//    catch (const Poco::Net::HostNotFoundException& exc)
//    {
//        ClientErrorEventArgs args(context, request, response, exc);
//        ofNotifyEvent(context.events.onHTTPClientError, args, this);
//    }
//    catch (const Poco::Net::HTTPException& exc)
//    {
//        ClientErrorEventArgs args(context, request, response, exc);
//        ofNotifyEvent(context.events.onHTTPClientError, args, this);
//    }
//    catch (const Poco::Exception& exc)
//    {
//        ClientErrorEventArgs args(context, request, response, exc);
//        ofNotifyEvent(context.events.onHTTPClientError, args, this);
//    }
//    catch (const std::exception& exception)
//    {
//        Poco::Exception exc(exception.what());
//        ClientErrorEventArgs args(context, request, response, exc);
//        ofNotifyEvent(context.events.onHTTPClientError, args, this);
//    }
//    catch (...)
//    {
//        Poco::Exception exc("Completely Unknown Exception");
//        ClientErrorEventArgs args(context, request, response, exc);
//        ofNotifyEvent(context.events.onHTTPClientError, args, this);
//    }
//}
//
//
//void BaseClient::addRequestFilter(AbstractRequestFilter* filter)
//{
//    _requestFilters.push_back(filter);
//}
//
//
//void BaseClient::addResponseFilter(AbstractResponseFilter* filter)
//{
//    _responseFilters.push_back(filter);
//}
//
//
//void BaseClient::removeRequestFilter(AbstractRequestFilter* filter)
//{
//    _requestFilters.erase(std::remove(_requestFilters.begin(),
//                                      _requestFilters.end(),
//                                      filter),
//                           _requestFilters.end());
//}
//
//
//void BaseClient::removeResponseFilter(AbstractResponseFilter* filter)
//{
//    _responseFilters.erase(std::remove(_responseFilters.begin(),
//                                       _responseFilters.end(),
//                                       filter),
//                           _responseFilters.end());
//}
//
//void BaseClient::addRequestStreamFilter(AbstractRequestStreamFilter* filter)
//{
//    _requestStreamFilters.push_back(filter);
//    addRequestFilter(filter);
//}
//
//
//void BaseClient::removeRequestStreamFilter(AbstractRequestStreamFilter* filter)
//{
//    removeRequestFilter(filter);
//
//    _requestStreamFilters.erase(std::remove(_requestStreamFilters.begin(),
//                                            _requestStreamFilters.end(),
//                                            filter),
//                                 _requestStreamFilters.end());
//}
//
//
//void BaseClient::addResponseStreamFilter(AbstractResponseStreamFilter* filter)
//{
//    _responseStreamFilters.push_back(filter);
//    addRequestFilter(filter);
//    addResponseFilter(filter);
//}
//
//
//void BaseClient::removeResponseStreamFilter(AbstractResponseStreamFilter* filter)
//{
//    removeRequestFilter(filter);
//    removeResponseFilter(filter);
//
//    _responseStreamFilters.erase(std::remove(_responseStreamFilters.begin(),
//                                             _responseStreamFilters.end(),
//                                             filter),
//                                 _responseStreamFilters.end());
//}
//
//
//} } // namespace ofx::HTTP
