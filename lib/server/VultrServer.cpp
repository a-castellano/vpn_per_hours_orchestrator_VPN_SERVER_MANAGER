// VultrServer.cpp
// Álvaro Castellano Vela 12/03/2016

//#define CURL_STATICLIB

#include "VultrServer.h"

#include <curl/curl.h>
#include <sstream>
#include <fstream>
#include <vector>

#include <boost/property_tree/json_parser.hpp>

VultrServer::VultrServer(const std::string &token) : Server(token) {}

const std::string VultrServer::serverType(void) { return std::string("Vultr"); }

VultrServer::~VultrServer() {}

const std::string VultrServer::zoneName() {
  switch (getZone()) {
  case 1:
    return std::string("US");
  case 2:
    return std::string("DE");
  case 4:
    return std::string("GB");
  case 5:
    return std::string("NL");
  case 7:
    return std::string("FR");
  case 8:
    return std::string("JP");
  case 9:
    return std::string("AU");
  }
  return std::string("");
}

bool VultrServer::curlGET(const std::string &url, std::stringstream &result) {
  CURL *curl;
  CURLcode res;

  std::stringstream header;
  std::ostringstream os;
  curl = curl_easy_init();

  struct curl_slist *chunk = NULL;

  if (curl) {
    header << "API-Key: " << VULTR_API_KEY;
    chunk = curl_slist_append(chunk, header.str().c_str());
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    //curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, -1); 
    curl_easy_setopt(curl, CURLOPT_DNS_LOCAL_IP4, "8.8.8.8");
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write);
    curl_easy_setopt(curl, CURLOPT_FILE, &os);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
      return false;
    }

    //curl_easy_cleanup(curl);
    curl_slist_free_all(chunk);
		curl_easy_cleanup(curl);
    result.str("");
    result << os.str();
  }
  return true;
}

bool VultrServer::curlPOST(const std::string &url, const std::string &POSTdata,
                           std::stringstream &result) {
  CURL *curl;
  CURLcode res;

  std::stringstream header;
  std::ostringstream os;

  curl = curl_easy_init();

  struct curl_slist *chunk = NULL;
  if (curl) {
    header << "API-Key: " << VULTR_API_KEY;

    chunk = curl_slist_append(chunk, header.str().c_str());
    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    chunk = curl_slist_append(chunk, "Host: api.vultr.com");
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, POSTdata.c_str());
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    //curl_easy_setopt(curl, CURLOPT_DNS_CACHE_TIMEOUT, -1); 
    curl_easy_setopt(curl, CURLOPT_DNS_LOCAL_IP4, "8.8.8.8");
    curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
    curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTPS);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write);
    curl_easy_setopt(curl, CURLOPT_FILE, &os);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
      return false;
    }

    
    curl_slist_free_all(chunk);
		curl_easy_cleanup(curl);  
    result.str("");
    result << os.str();
  }
  return true;
}

bool VultrServer::create() {

  std::stringstream header;
  std::ostringstream os;

  std::stringstream json;

  boost::property_tree::ptree regions;
  boost::property_tree::ptree::const_iterator end;

  std::string zone_name;

  std::string availability_request;
  std::stringstream availability_result;

  std::vector<std::string> currentDCID;
  std::string DCID;
  int CDIDId;

  std::string createServerRequest;
  std::string createServerPOSTdata;
  std::stringstream serverJSONSUBID;

	std::string serverSUBID;

	boost::property_tree::ptree jsonCreateServer;

	std::string createServerIPRequest;
	std::stringstream serverJSONIP;

	boost::property_tree::ptree ipv4;

	std::string serverIP;


  if (curlGET(std::string("https://api.vultr.com/v1/regions/list"), json)) {
    zone_name = zoneName();
    boost::property_tree::read_json(json, regions);

    end = regions.end();
    for (boost::property_tree::ptree::const_iterator it = regions.begin();
         it != end; ++it) {
      if (std::string(it->second.get_child("country").data()) == zone_name) {
        // Check availability for earch zone
        availability_request =
            std::string("https://api.vultr.com/v1/regions/availability?DCID=") +
            std::string(it->second.get_child("DCID").data());
        if (curlGET(std::string("https://api.vultr.com/v1/regions/list"),
                    availability_result)) {
          if (availability_result.str() != std::string("[]")) {
            currentDCID.push_back(
                std::string(it->second.get_child("DCID").data()));
          }
        }
      }
    } // for
    // Select one DCID
    if (currentDCID.size() == 0) {
			std::cout<<"NO HAY ZONAS "<<std::endl; 
      return false; // and write and erro msg
    } else {
      if (currentDCID.size() == 1) {
        DCID = currentDCID[0];
      } else {
        srand(time(NULL));
        CDIDId = rand() % (currentDCID.size());
        DCID = currentDCID[CDIDId];
      }
    }

    // first test
    createServerRequest = std::string("https://api.vultr.com/v1/server/create");
    createServerPOSTdata =
        std::string("VPSPLANID=29&OSID=160&SSHKEYID=56d08c10951a6&label=") + this->getServerName() + std::string("&DCID=") +
        std::string(DCID);

    if (curlPOST(createServerRequest, createServerPOSTdata, serverJSONSUBID)) {
			boost::property_tree::read_json(serverJSONSUBID, jsonCreateServer);
			serverSUBID = std::string(jsonCreateServer.get_child("SUBID").data());
			setMachineID(serverSUBID);
			setTrueZone(std::string(DCID));
			createServerIPRequest = std::string("https://api.vultr.com/v1/server/list_ipv4?SUBID=") + getMachineID();

			if (curlGET(createServerIPRequest, serverJSONIP))
			{
				boost::property_tree::read_json(serverJSONIP, ipv4);
				
				serverIP = std::string( ipv4.get_child(getMachineID().c_str()).begin()->second.get<std::string>("ip") );
				while(serverIP=="0")
			 	{
					if (curlGET(createServerIPRequest, serverJSONIP)) 
					{
						boost::property_tree::read_json(serverJSONIP, ipv4);
						serverIP = std::string( ipv4.get_child(getMachineID().c_str()).begin()->second.get<std::string>("ip") );
					}
				}
				setServerIP(std::string( serverIP ));
			}
    }
		else{
			return false;
		}
  }
  return true;
}

bool VultrServer::destroy() { return true; }

bool VultrServer::powerOn() { return true; }

bool VultrServer::powerOff() { return true; }
