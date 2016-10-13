// DatabaseHandler.h
// Álvaro Castellano Vela 24/02/2016


#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class DatabaseHandler
{

	public:
		DatabaseHandler( const std::string & , const unsigned int & , const std::string & , const std::string & , const std::string & );

		bool dataIsWellFormed( void );

		bool successConected( void );

		bool hasError( void );

		bool queryTest( void );

		unsigned int getServerZoneFromToken( const std::string &  );

		std::vector<std::string> getProvidersFromZone( const unsigned int & );

		std::string setServerName(const std::string & ,const unsigned int &);

		bool updateDBField(const std::string &, const std::string &,
				   const std::string & ,const std::string &);
		std::string getErrorMsg( void );


		std::vector<unsigned int> getVPNUsers(const std::string & );
		bool getVPNUserPassword(const unsigned int &, std::string *,
					std::string * );
	private:
		std::string address;
		std::string user;
		std::string pass;
		std::string database;

		bool dataWellFormed;
		bool connected;
		bool error;

		std::string errormsg;

                sql::Driver *driver;
                sql::Connection *con;
                sql::Statement *stmt;
                sql::ResultSet *res;


		bool connect( void );
		bool disconnect( void );

		std::string zone_initials[10] = {"","usa","ger","sgp","uk","net","can","fra","jap","au"};
};
