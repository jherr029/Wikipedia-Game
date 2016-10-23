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
std::string getArticles( std::string, std::list< std::queue < std::string > > &, int & );

int main( )
{
    std::list< std::queue <std::string> > listQueuArticles;

    std::string startFlag        = "Antartica";
    std::string endFlag          = "Purity";
    std::string completeFlag     = "incomplete";
    std::string quitFlag         = "no";

    std::ofstream outFile;

    int count = 0;

    outFile.open( "Data.txt", std::ios_base::app );

    //std::cout << "Starting Article: ";
    //std::cin >> startFlag;

    //std::cout << "Ending Article: ";
    //std::cin >> endFlag;
  
    callPythonScript( startFlag );

    while ( completeFlag != "complete" )
    {
       
        completeFlag = getArticles( endFlag, listQueuArticles, count ); 

        std::cout << listQueuArticles.front( ).front( ) << std::endl;
        outFile << listQueuArticles.front( ).front( ) << std::endl;

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
    std::cout << "Links visited: " << count << std::endl;
    outFile.close( );

    return 0;
}

void callPythonScript( std::string article )
{
    std::string wikiLink;

    wikiLink = addWikiLink( article );

    std::string command = "python test.py ";

    command += wikiLink;

    system( command.c_str( ) );

}

std::string getArticles( std::string endArticle, std::list< std::queue< std::string> > &articles, int &i )
{
    std::ifstream myFile;
    std::ofstream outFile;
    std::string line;
    std::string title;

    std::queue< std::string > titleQueue;

    //std::cout << "READING FROM FILE " << std::endl;

    myFile.open( "myFile" );
    outFile.open( "Data.txt", std::ios_base::app | std::ofstream::trunc );

    if ( myFile.is_open( ) )
    {
        while ( getline( myFile, line ) )
        {
            std::stringstream ss( line );
            ss >> title;
            

            if ( title.find( ":") != std::string::npos )
            {
                if ( title.find( "Category" ) != std::string::npos &&
                        title.find( "Help" ) == std::string::npos  )
                {
                    titleQueue.push( title );
                }
            }

            else
            {
                if ( title.find( "Main_Page" ) == std::string::npos )
                {
                    titleQueue.push( title );
                }

            }
            
            outFile << "\t" + title << std::endl;
            //std::cout << "BREAK?" << std::endl;
            
            //std::cout << "\t" << title << std::endl;
            i++;
            
            if ( endArticle == title )
            {
                std::cout << endArticle << " == " << title << std::endl;
                std::cout << "FOUND A MATCH" << std::endl;
                return "complete";
            }

            //std::cout << line << std::endl;
        }
    }
    
    //std::cout << "PUSHING THAT SHIT" << std::endl;
    articles.push_back( titleQueue );

    myFile.close( );
    outFile.close( );

    return "no_match";
}


std::string addWikiLink( std::string article )
{
    std::string test = "\"https://en.wikipedia.org/wiki/" + article + "\"";

    return test;
}
