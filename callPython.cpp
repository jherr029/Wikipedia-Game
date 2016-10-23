#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>

#include<list>
#include<stack>

std::string fixSpaces( std::string );
std::string addWikiLink( std::string );

void callPythonScript( std::string );
std::string getArticles( std::string );

int main( )
{

    std::string startFlag        = "Kakababu";
    std::string endFlag          = "Pattern";
    std::string completeFlag     = "incomplete";
    std::string quitFlag         = "no";

    //std::cout << "Starting Article: ";
    //std::cin >> startFlag;

    //std::cout << "Ending Article: ";
    //std::cin >> endFlag;
    
    callPythonScript( startFlag );
    getArticles( endFlag );
    
    std::cout << "Done" << std::endl;


    return 0;
}

void callPythonScript( std::string article )
{
    std::string wikiLink;

    wikiLink = addWikiLink( article );

    std::string endArticle = "Eating_disorder";

    std::string command = "python test.py ";

    command += wikiLink;

    system( command.c_str( ) );

}

std::string getArticles( std::string endArticle )
{
    std::ifstream myFile;
    std::string line;

    std::list<std::string> listOfTitles;

    myFile.open( "myFile" );

    if ( myFile.is_open( ) )
    {
        while ( getline( myFile, line ) )
        {
            listOfTitles.push_back( line );

            if ( endArticle == line )
            {
                std::cout << "FOUND A MATCH" << std::endl;
                return "match";
            }

            //std::cout << line << std::endl;
        }
    }

    myFile.close( );

    return "no_match";
}


std::string addWikiLink( std::string article )
{
    return ( "https://en.wikipedia.org/wiki/" + article );
}
