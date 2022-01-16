#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <vector>
# include <fstream>

# include "../headers/Server_t.hpp"

class Config
{
    private:
        std::vector<Server_t> _servers;

/* Private functions used for parsing */
        void check_comment(std::fstream &file, std::string &word) const;
        void next_word(std::fstream &file, std::string &word) const;

        void check_server(std::fstream &file, std::string &word) const;
        void parse_server(std::fstream &file, std::string &word);

    public:
        Config(const char * path);
        Config(Config &copy);
        Config  &operator=(Config &other);
        virtual ~Config();

       	class FileOpenException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        class ExpectedServerException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        std::vector<Server_t> get_servers();
};
#endif
