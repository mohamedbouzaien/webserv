#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#define SIZE 12

char *css_typo(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	std::replace(s.begin(), s.end(), ' ', '_');
	char *ss =(char *) s.c_str();
	return (ss);
}

int main() {
	char *error_name[SIZE] = {"Proxy Authentication Required", "Request Timeout", "Conflict", "Gone", "Length Required", "Precondition Failed", "Payload Too Large", "URI Too Long", "Unsupported Media Type", "Range Not Satisfiable", "Expectation Failed", "I'm a teapot"};
	char *error_css[SIZE];
	for (int i = 0; i < 12; i++)
		error_css[i] = css_typo(error_name[i]);
	int i = 0;
	while (i < 11) {
		std::string content = "<!DOCTYPE html>\n<html lang=\"en\">\n\n    <head>\n        <meta charset=\"utf-8\">\n        <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\n        <title>Webserv Error</title>\n\n        <link href=\"https://fonts.googleapis.com/css?family=Montserrat:200,400,700\" rel=\"stylesheet\">\n        <style>\n                * {\n                -webkit-box-sizing: border-box;\n                        box-sizing: border-box;\n                }\n\n                body {\n                padding: 0;\n                margin: 0;\n                }\n\n                #" + std::string(error_css[i]) + " {\n                position: relative;\n                height: 100vh;\n                }\n\n                #" + std::string(error_css[i]) + " ." + std::string(error_css[i]) + " {\n                position: absolute;\n                left: 50%;\n                top: 50%;\n                -webkit-transform: translate(-50%, -50%);\n                    -ms-transform: translate(-50%, -50%);\n                        transform: translate(-50%, -50%);\n                }\n\n                ." + std::string(error_css[i]) + " {\n                max-width: 520px;\n                width: 100%;\n                line-height: 1.4;\n                text-align: center;\n                }\n\n                ." + std::string(error_css[i]) + " ." + std::string(error_css[i]) + "-" + std::to_string(407 + i) + " {\n                position: relative;\n                height: 200px;\n                margin: 0px auto 20px;\n                z-index: -1;\n                }\n\n                ." + std::string(error_css[i]) + " ." + std::string(error_css[i]) + "-" + std::to_string(407 + i) + " h1 {\n                font-family: 'Montserrat', sans-serif;\n                font-size: 236px;\n                font-weight: 200;\n                margin: 0px;\n                color: #211b19;\n                text-transform: uppercase;\n                position: absolute;\n                left: 50%;\n                top: 50%;\n                -webkit-transform: translate(-50%, -50%);\n                    -ms-transform: translate(-50%, -50%);\n                        transform: translate(-50%, -50%);\n                }\n\n                ." + std::string(error_css[i]) + " ." + std::string(error_css[i]) + "-" + std::to_string(407 + i) + " h2 {\n                font-family: 'Montserrat', sans-serif;\n                font-size: 28px;\n                font-weight: 400;\n                text-transform: uppercase;\n                color: #211b19;\n                background: #fff;\n                padding: 10px 5px;\n                margin: auto;\n                display: inline-block;\n                position: absolute;\n                bottom: 0px;\n                left: 0;\n                right: 0;\n                }\n\n                @media only screen and (max-width: 767px) {\n                ." + std::string(error_css[i]) + " ." + std::string(error_css[i]) + "-" + std::to_string(407 + i) + " h1 {\n                    font-size: 148px;\n                }\n                }\n\n                @media only screen and (max-width: 480px) {\n                ." + std::string(error_css[i]) + " ." + std::string(error_css[i]) + "-" + std::to_string(407 + i) + " {\n                    height: 148px;\nmargin: 0px auto 10px;\n                }\n\n            ." + std::string(error_css[i]) + " ." + std::string(error_css[i]) + "-" + std::to_string(407 + i) + " h1 {\n                    font-size: 86px;\n                }\n                ." + std::string(error_css[i]) + " ." + std::string(error_css[i]) + "-" + std::to_string(407 + i) + " h2 {\n                    font-size: 16px;\n                }\n                ." + std::string(error_css[i]) + " a {\n                    padding: 7px 15px;\n                    font-size: 14px;\n                }\n                }\n        </style>\n        <!--[if lt IE 9]>\n            <script src=\"https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js\"></script>\n            <script src=\"https://oss.maxcdn.com/respond/1.4.2/respond.min.js\"></script>\n            <![endif]-->\n\n    </head>\n\n    <body>\n\n        <div id=\"" + std::string(error_css[i]) + "\">\n            <div class=\"" + std::string(error_css[i]) + "\">\n				<div class=\"" + std::string(error_css[i]) + "-" + std::to_string(407 + i) + "\">\n                    <h1>Oops!</h1>\n                    <h2>" + std::to_string(407 + i) + " - " + std::string(error_name[i]) + "</h2>\n                </div>\n\n            </div>\n        </div>\n\n    </body>\n\n</html>";
		std::ofstream outfile (std::to_string(407 + i) + std::string(".html"));
		outfile << content << std::endl;
		outfile.close();
		i++;

	};
}
