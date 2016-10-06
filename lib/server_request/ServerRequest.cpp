// serverRequest.h
// Álvaro Castellano Vela 23/02/2016

#include "ServerRequest.h"
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <VPNLock.h>

extern VPNLock memoryLock;

ServerRequest::ServerRequest( const std::string & request )
{
	boost::split(processedRequest,request,boost::is_any_of("/"));
	correctRequest = false;
        if ( processedRequest.size() == 2 )
	{
		if ( processedRequest[0] == "create" || processedRequest[0] == "destroy" )
		{
			if ( processedRequest[1].size() == 60 )
			{
				correctRequest = true;
			}
		}
	}
}


ServerRequest::ServerRequest( const std::string & request, const int & threaded )
{
  memoryLock.getLock();
	boost::split(processedRequest,request,boost::is_any_of("/"));
  memoryLock.releaseLock();
	correctRequest = false;
        if ( processedRequest.size() == 2 )
	{
		if ( processedRequest[0] == "create" || processedRequest[0] == "destroy" )
		{
			if ( processedRequest[1].size() == 60 )
			{
				correctRequest = true;
			}
		}
	}
}



const bool ServerRequest::isCorrect( void )
{
	return correctRequest;	
}

const std::string ServerRequest::getCommand( void )
{
	if ( this->isCorrect() )
	{
		return processedRequest[0];
	}
	else
	{
		return "";
	}
}

const std::string ServerRequest::getToken( void )
{
        if ( this->isCorrect() )
        {
                return processedRequest[1];
        }
        else
        {
                return "";
        }
}
