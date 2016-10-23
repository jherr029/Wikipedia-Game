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
std::string getArticles( std::string, std::list< std::queue < std::string > > &, int &, std::string );
bool checkForRepeats( );


int main( )
{
    std::list< std::queue <std::string> > listQueuArticles;

    std::string startFlag        = "";
    std::string endFlag          = "";
    std::string completeFlag     = "incomplete";
    std::string quitFlag         = "no";

    std::ofstream outFile1;

    int count = 0;

    outFile1.open( "Data.txt", std::ofstream::trunc );

    std::cout << "Starting Article: ";
    std::cin >> startFlag;

    outFile1 << startFlag << std::endl;

    std::cout << "Ending Article: ";
    std::cin >> endFlag;
    
    std::cout << std::endl;
  
    callPythonScript( startFlag );

    std::string changeMe = startFlag;

    while ( completeFlag != "complete" )
    {
       
        completeFlag = getArticles( endFlag, listQueuArticles, count, changeMe ); 

        std::cout << listQueuArticles.front( ).front( ) << "!" << std::endl;
        changeMe = listQueuArticles.front( ).front( );
        outFile1 << listQueuArticles.front( ).front( ) << "!!!" << std::endl;

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
    std::cout << "\tLinks visited: " << count << std::endl;

    outFile1.close( );

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

std::string getArticles( std::string endArticle, std::list< std::queue< std::string> > &articles, int &i, std::string changeMe )
{
    std::ifstream myFile;
    std::ofstream outFile;
    std::string line;
    std::string title;

    std::queue< std::string > titleQueue;

    //std::cout << "READING FROM FILE " << std::endl;

    myFile.open( "myFile" );
    outFile.open( "Data.txt", std::ios_base::app );

    outFile << "\n" << changeMe << std::endl;

    if ( myFile.is_open( ) )
    {
        while ( getline( myFile, line ) )
        {
            std::stringstream ss( line );
            ss >> title;
            

            if ( title.find( ":" ) != std::string::npos ) 
            {
                if ( title.find("Category") != std::string::npos 
                        && title.find("Help") == std::string::npos ) 
                {
                    titleQueue.push( title );

                    outFile << "\t" + title << std::endl;
                 //   std::cout << " @ " << title << std::endl;
                }
            }

            else
            {
                if ( title != "Main_Page"  )
                {

		            if (title.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
		            { }

                    else
                    {
                        titleQueue.push( title );

                        outFile << "\t" + title << std::endl;
                        //std::cout << " # " << title << std::endl;
                    }
                }

            }
            
            i++;
            
            if ( endArticle == title )
            {

                articles.push_back( titleQueue );

                std::cout << "\n\t\t" << articles.front( ).back( ) << std::endl;
                std::cout << "\t\t\t" << endArticle << " == " << title << std::endl;
                std::cout << "\t\t\t\tFOUND A MATCH" << std::endl;

                myFile.close( );
                outFile.close( );

                return "complete";
            }

        }
    }

    std::cout << std::endl;
    
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
