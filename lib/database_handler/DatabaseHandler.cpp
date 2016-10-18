// DatabaseHandler.cpp
// Álvaro Castellano Vela 24/02/2016

#define BOOST_SP_USE_QUICK_ALLOCATOR
#ifndef DATABASEHANDLER
#include "DatabaseHandler.h"
#endif

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <boost/regex.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

DatabaseHandler::DatabaseHandler( const std::string & givenHost , const unsigned int & givenPort , const std::string & givenUser , const std::string & givenPass , const std::string & givenDatabase)
:pass(givenPass), database(givenDatabase), connected(false) 
{
	res = NULL;
	stmt = NULL;
	con = NULL;

	// Check that the host and the port are valid
	boost::regex IPPattern("(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])");

	boost::regex URLPattern("([\\da-z\\.-]+)\\.([a-z\\.]{2,6})|(localhost)");

	std::stringstream ss;

	dataWellFormed = true;
	error = false;
	errormsg = std::string("");

	if( !( boost::regex_match (givenHost , IPPattern) || boost::regex_match (givenHost , URLPattern) ) || givenUser.length() > 17)
	// If givenHost isn't an IP address maybe it's a domain name
	{
		dataWellFormed = false;
	}
	else
	{
		ss << "tcp://" << givenHost << ":" << givenPort;
		address = ss.str();
		user = givenUser;
	}

}

bool DatabaseHandler::dataIsWellFormed( void )
{
	return dataWellFormed;
}

bool DatabaseHandler::successConected( void )
{
	if ( dataWellFormed )
	{
        	return connected;
	}
	else
	{
		return false;
	}
}

bool DatabaseHandler::connect( void )
{
	try {
		driver = get_driver_instance();
		con = driver->connect(address, user, pass);
		con->setSchema(database);
		stmt = con->createStatement();
	} catch ( sql::SQLException &e ){
		connected = false;
		error = true;
		errormsg = std::string("Database conenction failed");
		std::cout << "# ERR: " << e.what()<<std::endl;
		return false;
	}
	error = false;
	connected = true;
	return true;
}

bool DatabaseHandler::disconnect( void )
{
	try {
		if(res)
		{
			res->first();
			delete res;
		}
    delete stmt;
		delete con;
		res = NULL;
		stmt = NULL;
		con = NULL;
		connected = false;

	} catch ( sql::SQLException &e ){
		std::cout << "# ERR: " << e.what()<<std::endl;
		return false;
	}
	return true;
}

bool DatabaseHandler::queryTest( void )
{
	connect();
	if ( connected )
	{
		res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
	}
	return disconnect();
}

bool DatabaseHandler::hasError( void )
{
	return error;
}

std::string DatabaseHandler::getErrorMsg( void )
{
	return errormsg;
}

unsigned int  DatabaseHandler::getServerZoneFromToken( boost::shared_ptr<std::string> token )
{
	std::stringstream query;
  unsigned int zone;
  query << "SELECT zone FROM servers WHERE token='" << *token << "' LIMIT 1";
	connect();
	if ( connected )
	{
		this->res = stmt->executeQuery( query.str() );
		if (this->res->next())
		{
			zone = std::stoi( this->res->getString("zone") );
      while(this->res->next());
		}
		else
		{
			disconnect();
			error = true;
			errormsg = std::string("Query response is empty.");
			zone = 0;
		}
	}
	else
	{
		std::cerr << "Failed to conenct" << std::endl;
		error = true;
		errormsg = std::string("Failed to conenct.");
    zone = 0;
	}

	disconnect();

	return zone;
}

std::vector<std::string> DatabaseHandler::getProvidersFromZone( const unsigned int & zone_id )
{
	std::vector<std::string> providers;
	std::stringstream query;

	if ( database == "vpn_zones" )
	{
		connect();

		if ( connected )
		{
			query << "SELECT provider FROM zones WHERE zone_id=" << zone_id << " ;";
			res = stmt->executeQuery( query.str() );
			while ( res->next() )
			{
				providers.push_back( res->getString("provider") );
			}
		disconnect();
		}

	}//providers will be emty
	return providers;
}

boost::shared_ptr<std::string> DatabaseHandler::setServerName(boost::shared_ptr<std::string> server_token ,const unsigned int & zone_id)
{
	std::stringstream user_query;
	std::stringstream servers_query;

	std::string  username;
	std::set<std::string> server_names;

	std::stringstream candidateURL;
	std::string  currentURL;
  boost::shared_ptr<std::string> finalURL;
	bool finded;

	user_query << "SELECT username FROM users JOIN servers WHERE servers.token='" << *server_token << "' LIMIT 1";
	connect();
	if ( connected )
	{
		this->res = stmt->executeQuery( user_query.str() );
		if (this->res->next())
		{
			username = this->res->getString("username");
		}
	}
	disconnect();

	servers_query << "SELECT name FROM servers WHERE zone=" << zone_id << " AND token <> '" << *server_token <<"'";
	connect();
	if ( connected )
	{
		this->res = stmt->executeQuery( servers_query.str() );
		while ( res->next() )
		{
			server_names.insert( res->getString("name") );
		}
	}
	res->first();
	disconnect();

	for (int i = 1 ,finded = false; finded==false ; i++){
		candidateURL << zone_initials[zone_id] << i << std::string(".") << username << ".vpn.windmaker.net";
		if( server_names.find(candidateURL.str()) == server_names.end() )
		{
			finded = true;
			finalURL = boost::make_shared<std::string>(candidateURL.str());

		}
		candidateURL.str("");
	}

	return finalURL;
	} //DatabaseHandler::setServerName

bool DatabaseHandler::updateDBField(boost::shared_ptr<std::string> token, const std::string &field, const std::string &type ,boost::shared_ptr<std::string> value)
{
	std::stringstream query;
	if (type == std::string("string"))
	{
		query << "UPDATE servers SET " << field << " = '" << *value << "' WHERE token='" << *token << "'";
	}
	else
	{
		query << "UPDATE servers SET " << field << " = " << *value << " WHERE token='" << *token << "'";
	}
	connect();
	if ( connected )
	{
		stmt->execute( query.str() );
	}
	disconnect();
	return true;
}

bool DatabaseHandler::updateDBField( boost::shared_ptr<std::string> token, const std::string &field, const std::string &type ,const std::string & value )
{
  boost::shared_ptr<std::string> value_pointer;
  bool toReturn;

  value_pointer = boost::make_shared<std::string>(value);
  toReturn = this->updateDBField(token, field, type, value_pointer);
  value_pointer.reset();

  return toReturn;
}

std::vector<unsigned int> DatabaseHandler::getVPNUsers(const std::string & token)
{
	unsigned int serverID=0;
	unsigned int user_candidate;

	std::vector<unsigned int> groups;
	std::vector<unsigned int> users;

	std::stringstream query;

	query << "SELECT id FROM servers WHERE token='" << token << "';";
	connect();
	if (connected)
	{
		this->res = stmt->executeQuery( query.str() );
		if (this->res->next())
		{
			serverID = std::stoi(this->res->getString("id"));
		}
	}
	disconnect();

	query.clear();
	query.str(std::string());
	//Get groups
	if (serverID){
		query << "SELECT vpn_group_id FROM serversvpngroups WHERE vpn_server_id="<< serverID;
		connect();
		if( connected )
		{
			this->res = stmt->executeQuery( query.str() );
			while ( this->res->next() )
			{
				groups.push_back( std::stoi(res->getString("vpn_group_id")) );
			}
		}
		disconnect();
		query.clear();
		query.str(std::string());
	}
	if (groups.size()) //Select Users from groups
	{
		for (unsigned int group : groups) {
			query << "SELECT vpn_user_id FROM vpnusersgroups  WHERE vpn_group_id=" << group ;
			connect();
			if( connected )
			{
				this->res = stmt->executeQuery( query.str() );
				while ( this->res->next() ){
					user_candidate = std::stoi(res->getString("vpn_user_id"));
					if ( std::find(users.begin(), users.end(), user_candidate) == users.end() )
					{
						users.push_back(user_candidate);
					}
				}
			}
			disconnect();
			query.clear();
			query.str(std::string());
		}
	}
	else{ //no groups, all users allowed
		query << "SELECT vpnusers.id FROM vpnusers JOIN servers WHERE servers.token='" << token << "'";
		connect();
		if( connected )
		{
			this->res = stmt->executeQuery( query.str() );
			while ( this->res->next() ){
				users.push_back( std::stoi(res->getString("id")) );
			}
		}
		disconnect();
		query.clear();
		query.str(std::string());
	}
	return users;
}

bool DatabaseHandler::getVPNUserPassword(const unsigned int & userID, std::string *username , std::string
																								*password)
{
	std::stringstream query;

	query << "SELECT name,password FROM vpnusers WHERE id =" << userID;
	connect();
	if( connected )
	{
		this->res = stmt->executeQuery( query.str() );
		this->res->next();
		*username = res->getString("name"); 
		*password = res->getString("password");
	}
	else
	{
		return false;
	}
	disconnect();
	return true;
}

