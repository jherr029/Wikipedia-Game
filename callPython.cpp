#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>

#include<list>
#include<queue>

std::string fixSpaces( std::string );
std::string addWikiLink( std::string );

void callPythonScript( std::string );
std::string getArticles( std::string, std::list< std::queue < std::string > > & );

int main( )
{
    std::list< std::queue <std::string> > listQueuArticles;

    std::string startFlag        = "Kakababu";
    std::string endFlag          = "Pattern";
    std::string completeFlag     = "incomplete";
    std::string quitFlag         = "no";

    //std::cout << "Starting Article: ";
    //std::cin >> startFlag;

    //std::cout << "Ending Article: ";
    //std::cin >> endFlag;
  
    callPythonScript( startFlag );

    while ( completeFlag != "complete" )
    {
       
        completeFlag = getArticles( endFlag, listQueuArticles ); 
        std::cout << listQueuArticles.front( ).front( ) << std::endl;

        callPythonScript( listQueuArticles.front( ).front( ) );
        
        if ( !listQueuArticles.front( ).empty( ) )
            listQueuArticles.front( ).pop( );
        else
        {
            completeFlag = "complete";
            std::cout << "EMPTY EMPTY EMPTY" << std::endl;
        }
    }
   
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

std::string getArticles( std::string endArticle, std::list< std::queue< std::string> > &articles )
{
    std::ifstream myFile;
    std::string line;
    std::string title;

    std::queue< std::string > titleQueue;

    //std::cout << "READING FROM FILE " << std::endl;

    myFile.open( "myFile" );

    if ( myFile.is_open( ) )
    {
        while ( getline( myFile, line ) )
        {
            std::stringstream ss( line );
            ss >> title;
            //std::cout << "INSERTING" << std::endl;
            
            //std::cout << " " << title << std::endl;
            titleQueue.push( title );
            //std::cout << "BREAK?" << std::endl;

            if ( endArticle == title )
            {
                std::cout << "FOUND A MATCH" << std::endl;
                return "match";
            }

            //std::cout << line << std::endl;
        }
    }
    
    //std::cout << "PUSHING THAT SHIT" << std::endl;
    articles.push_back( titleQueue );

    myFile.close( );

    return "no_match";
}


std::string addWikiLink( std::string article )
{
    std::string test = "\"https://en.wikipedia.org/wiki/" + article + "\"";

    return test;
}
