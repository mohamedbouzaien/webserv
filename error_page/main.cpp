#include <iostream>
#include <fstream>
#include<sstream>
#define SIZE 18

void	replace(std::ifstream *ifs, std::ofstream *ofs, std::string s1, std::string s2)
{
	std::string	line;
	size_t	location;
	while (std::getline(*ifs, line))
	{
		location = line.find(s1);
		while  (location != std::string::npos)
		{
			*ofs << line.substr(0, location);
			*ofs << s2;
			line = line.substr(location + s1.size(), line.size());
			location = line.find(s1);
		}
		*ofs << line << std::endl;
	}
}

std::string replaceAll( std::string s, const std::string &search, const std::string &replace ) {
    for( size_t pos = 0; ; pos += replace.length() ) {
        // Locate the substring to replace
        pos = s.find( search, pos );
        if( pos == std::string::npos ) break;
        // Replace by erasing and inserting
        s.erase( pos, search.length() );
        s.insert( pos, replace );
    }
	return (s);
}

int	main(int ac, char **av)
{
	std::string name[SIZE] = {"Unauthorized", "Payment Required ", "Forbidden", "Not Found", "Method Not Allowed", "Not Acceptable", "Proxy Authentication Required", "Request Timeout", "Conflict", "Gone", "Length Required", "Precondition Failed", "Payload Too Large", "URI Too Long", "Unsupported Media Type", "Range Not Satisfiable", "Expectation Failed", "I'm a teapot"};
	std::ifstream ifs("400.html");
	std::string str;
   if(ifs) {
	   std::ostringstream ss;
      ss << ifs.rdbuf(); // reading data
      str = ss.str();
   }
	int i = 0;
	while (i < SIZE) {
		std::string ofile = std::to_string(401 + i) + ".html";
		std::ofstream ofs(ofile.c_str());
		std::string result = replaceAll(str, "400 - Bad Request", std::to_string(401 + i) + " - " + name[i]);
		ofs << result;
		ofs.close();
		i++;
	}
}
