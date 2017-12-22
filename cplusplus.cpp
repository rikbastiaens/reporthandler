#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#define MAX_LEN 1000

using namespace std;
using namespace cgicc;

FILE *pConfLoc;
char line[MAX_LEN];
char s1[MAX_LEN];

std::string executeReport(std::string report, std::string var) {
	if((pConfLoc = fopen("reports/medtronic.conf", "r")) == NULL) {
		cout << "Kan rapportage template niet openen." << endl;	
	}
	else {
		while(fgets(line, sizeof line, pConfLoc)) {
			if(*line == '#') {
				continue;
			}
			std::string res(line);
			if(res.substr(0,(size_t)(var.length())) == var) {
				res.erase(0,(size_t)(var.length()+1));
				return res;
			}
		}
	}
}

int main() {
	Cgicc formData;
	unsigned int colcnt;

	cout << "Content-type:text/html\r\n\r\n";
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<meta charset=\"UTF-8\">\n";
	cout << "<title></title>\n";
	cout << "</head>\n";
	cout << "<body>\n";

	try {
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	sql::ResultSetMetaData *rsmd;

	driver = get_driver_instance();
	con = driver->connect("tcp://192.168.162.56:3306", "rik", "Parkway91");
	
	con->setSchema("multipressdata");
	stmt = con->createStatement();
	form_iterator fi = formData.getElement("klantnr");

	std::string table = executeReport("medtronic","table");
	std::string fields = executeReport("medtronic","fields");
	std::cout << table;
	std::cout << fields;
	
	if ( !fi->isEmpty() && fi != (*formData).end()) {
		//res = stmt->executeQuery("SELECT "+fields+" FROM "+table+" WHERE KLANTNR = \'"+**fi+"\'");
		res = stmt->executeQuery("SELECT "+fields+" FROM "+table);
		//rsmd = res->getMetaData();
		//colcnt = rsmd->getColumnName();
	}
	
	while (res->next()) {
		
		cout << res->getString(1) << "<br>";	
	}
	delete res;
	delete stmt;
	delete con;
	}
	catch (sql::SQLException &e) {
		
	}
	cout << "</body>\n";
	cout << "</html>\n";
 	return 0;
}

