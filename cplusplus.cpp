#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#include <mysql_connection.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/resultset_metadata.h>

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
	string fp = "reports/"+report+".conf";
	if((pConfLoc = fopen(fp.c_str(), "r")) == NULL) {
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
	cout << "<script src=\"../js/functions.js\"></script>"; 
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
	//form_iterator fi = formData.getElement("klantnr");

	std::string table = executeReport("statussen","table");
	std::string fields = executeReport("statussen","fields");
	std::string params = executeReport("statussen","params");
	
	std::cout << "<input placeholder=\"Zoek op klant...\" onkeyup=\"search(this,0)\" />";
	std::cout << "<input placeholder=\"Zoek op ordernummer...\" onkeyup=\"search(this,1)\" />";

	//if ( !fi->isEmpty() && fi != (*formData).end()) {
	string query;
	if(params != "") {
		query = "SELECT "+fields+" FROM "+table+" WHERE "+params;
	}
	else {
		query = "SELECT "+fields+" FROM "+table;
	}
	res = stmt->executeQuery(query.c_str());
       	rsmd = res->getMetaData();
	int colcnt = rsmd->getColumnCount();
	//}
	std::cout << "<table id=\"resultTable\">";
	std::cout << "<tr>";
	
 	for(int j = 1; j <= colcnt; j++) 
	{
		string colName = rsmd->getColumnName(j);
		//colName = "col";
		std::cout << "<th>" << colName << "</th>";
	}		
	std::cout << "</tr>";
	while (res->next()) {
		std::cout << "<tr>";
		for(int i = 1; i <= colcnt; i++) {
			cout << "<td>" << res->getString(i) << "</td>";
		}
		std::cout << "</tr>";
	}
	std::cout << "</table>";
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

