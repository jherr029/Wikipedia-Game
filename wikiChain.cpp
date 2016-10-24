#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>

#include <list>
#include <queue>
#include <stack>
#include <vector>

void getInput( std::string & , std::string & );
std::string fixSpaces( std::string );
std::string addWikiLink( std::string );

void callPythonScript( std::string );

std::string getArticles( std::string, std::list< std::vector < std::string > > &, int &, std::string );
std::vector< std::string > getLinks( int & );
std::string scanArticle( std::vector< std::string >, std::list< std::vector< std::string > > & , std::string );

bool checkForRepeats( );


int main( )
{
    std::list< std::vector <std::string> > listQueuArticles;
    std::vector< std::string > tmp;

    std::string startFlag        = "";
    std::string endFlag          = "";
    std::string completeFlag     = "incomplete";
    std::string quitFlag         = "no";

    std::ofstream outFile;
    std::ofstream resetFile;

    resetFile.open( "Data.txt", std::ofstream::trunc );
    resetFile.close( );

    int count = 0;

    outFile.open( "Articles.txt", std::ofstream::trunc );

    getInput( startFlag, endFlag );

    callPythonScript( startFlag );

    std::string changeMe = startFlag;

    tmp.push_back( startFlag );
    listQueuArticles.push_front( tmp );

    outFile << startFlag << std::endl;

    while ( completeFlag != "complete" || listQueuArticles.empty( ) )
    {
        completeFlag = getArticles( endFlag, listQueuArticles, count, changeMe );

        listQueuArticles.pop_front( );

        if ( completeFlag != "complete" )
        {
            // Name of the article that is currently being visited
            //std::cout << "\t" << listQueuArticles.front( ).back( ) << std::endl;
            //outFile << "\t" << listQueuArticles.front( ).back( ) << std::endl;
            changeMe = listQueuArticles.front( ).back( );

            callPythonScript( listQueuArticles.front( ).back( ) );

        }
    }

    std::cout << "Done" << std::endl;
    std::cout << "\tLinks visited: " << count << std::endl;
    std::cout << listQueuArticles.size( ) << std::endl;
    std::cout << "Printing the path..." << listQueuArticles.back( ).size( ) << std::endl;

    for ( unsigned int i = 0; i < listQueuArticles.back( ).size( ); i++ )
    {
        std::cout << listQueuArticles.back( ).at( i ) << std::endl;
    }

    outFile.close( );

    return 0;
}

void getInput( std::string & start, std::string & end )
{
    std::cout << "Starting Article: ";
    std::cin >> start;

    std::cout << "Ending Article: ";
    std::cin >> end;

    std::cout << std::endl;

}

void callPythonScript( std::string article )
{
    std::string wikiLink;

    wikiLink = addWikiLink( article );

    std::string command = "python linkFetcher.py ";

    command += wikiLink;

    system( command.c_str( ) );

}

std::string getArticles( std::string endArticle, std::list< std::vector< std::string> > & articles, int &i, std::string changeMe )
{
    std::ofstream outFile;
    std::string line;
    std::string title;

    std::string quit;

    std::vector< std::string> vec = getLinks( i );

    std::string status = scanArticle( vec, articles, endArticle );

    if ( status == "complete")
    {
        std::cout << "MATCH FOUND" << std::endl;
        return status;
    }

    //outFile.open( "Data.txt", std::ios_base::app );

    //outFile << "\n" << changeMe << std::endl;

    outFile.close( );

    vec.clear( );
    return "no_match";
}


std::vector< std::string > getLinks( int &i )
{
    std::string title;
    std::string line;
    std::ifstream myFile;
    std::vector< std::string > vec;

    myFile.open( "myFile" );

    if ( myFile.is_open( ) )
    {
        while ( getline( myFile, line ) )
        {
            std::stringstream ss( line );
            ss >> title;

            if ( title.find( ":" ) != std::string::npos )
            {
                if ( title.find("Category") != std::string::npos && title.find("Help") == std::string::npos )
                {
                    vec.push_back( title );
                    //outFile << "\t" + title << std::endl;
                    //std::cout << " @ " << title << std::endl;
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
                        vec.push_back( title );
                        //outFile << "\t" + title << std::endl;
                       //std::cout << " # " << title << std::endl;
                    }
                }
            }

            i++;
        }
    }

    myFile.close( );
    return vec;
}

std::string scanArticle( std::vector< std::string > vec, std::list< std::vector< std::string > > & articles, std::string endArticle )
{
    std::ofstream outFile;

    std::string resultFlag = "incomplete";

    outFile.open( "Data.txt", std::ios_base::app );

    //std::cout << "Scanning Article: " << articles.front( ).back( ) << std::endl;
    outFile << articles.front( ).back( ) << std::endl;

    for ( unsigned int i = 0; i < vec.size( ); i++ )
    {
        std::vector< std::string > tmp = articles.front( );

        tmp.push_back( vec[i] );
        articles.push_back( tmp );

        outFile << "\t" << vec[i] << std::endl;

        if ( endArticle == vec[i] )
        {
            std::cout << "MATCH" << std::endl;
            i = vec.size( ) + 10;

            resultFlag = "complete";
        }

    }

    outFile.close( );
    vec.clear( );

    return resultFlag;
}

std::string addWikiLink( std::string article )
{
    std::string test = "\"https://en.wikipedia.org/wiki/" + article + "\"";

    return test;
}
