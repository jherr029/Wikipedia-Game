// To save time, stop calling the python script to get the links
// if the file already exist. That should save a lot of time
// Since grabbing the links can take a good amount of time
// So, only grab if the link file does not exist
// Priority -- medium


#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <list>
#include <queue>
#include <stack>
#include <vector>

void getInput( std::string & , std::string & );
std::string fixSpaces( std::string );
std::string addWikiLink( std::string );

void callPythonScript( std::string );

std::string getArticles( std::string, std::list< std::vector < std::string > > &, int & );
std::vector< std::string > getLinks( int & );
std::string scanArticle( std::vector< std::string >, std::list< std::vector< std::string > > & , std::string );
std::string getPath( std::string );
bool doesFileExist( std::string  );

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

    tmp.push_back( startFlag );
    listQueuArticles.push_front( tmp );

    outFile << startFlag << std::endl;
    std::cout << startFlag << std::endl;

    while ( completeFlag != "complete" || listQueuArticles.empty( ) )
    {
        completeFlag = getArticles( endFlag, listQueuArticles, count );

        if ( !listQueuArticles.empty( ) )
            listQueuArticles.pop_front( );

        if ( completeFlag != "complete" )
        {
            std::string currentArticle = listQueuArticles.front( ).back( );

            // Name of the article that is currently being visited
            std::cout << currentArticle << std::endl;

            if ( !doesFileExist( currentArticle ) )
                 callPythonScript( currentArticle );
            else
                std::cout << "Not calling python script - file exist" << std::endl;
        }
    }

    std::cout << "Links visited: " << count << std::endl;
    std::cout << "Size of list: " << listQueuArticles.size( ) << std::endl;
    std::cout << "Printing the path..." << listQueuArticles.back( ).size( ) << std::endl << std::endl;

    std::string space = "";
    std::string spaceInc = " ";
    for ( unsigned int i = 0; i < listQueuArticles.back( ).size( ); i++ )
    {
         std::cout << space << listQueuArticles.back( ).at( i ) << std::endl;
         space = space + spaceInc;
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

std::string getArticles( std::string endArticle, std::list< std::vector< std::string> > & articles, int &i )
{
    std::ofstream outFile;
    std::string line;
    std::string title;

    std::string quit;

    std::vector< std::string> vec = getLinks( i );

    std::string status = scanArticle( vec, articles, endArticle );

    if ( status == "complete")
        return status;

    //outFile.open( "Data.txt", std::ios_base::app );

    //outFile << "\n" << changeMe << std::endl;

    vec.clear( );
    return "no_match";
}


std::vector< std::string > getLinks( int &i )
{
    std::string title;
    std::string line;
    std::ifstream myFile;
    std::vector< std::string > vec;


    // TO DO. If that fileExistFlag is true
    // go to the article folder and reading from the file
    // instead of grabbing the links against which seems to
    // take awhile.
    // Reading from the file should be quicker
    // Probably not within here. Make a new paremter value for the getArticles
    // section


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

                    else                    {
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
    std::string fileExistFlag = "false";

    std::string fileNameStr = getPath( articles.front( ).back( ) );

    const char * fileName = fileNameStr.c_str( );

    if ( !doesFileExist( fileNameStr ) )
    {
        outFile.open( fileName, std::ios_base::app );
        //std::cout << "Scanning Article: " << articles.front( ).back( ) << std::endl;
    }

    else
    {
        std::cout << "\tFile exists -> " << fileName << std::endl;
        fileExistFlag = "true";
    }

    for ( unsigned int i = 0; i < vec.size( ); i++ )
    {
        std::vector< std::string > tmp = articles.front( );

        if ( resultFlag != "complete" )
        {
            tmp.push_back( vec[i] );
            articles.push_back( tmp );
        }

        if ( fileExistFlag == "false")
            outFile << vec[i] << std::endl;

        if ( endArticle == vec[i] )
        {
            std::cout << "\nMATCH" << std::endl;
            resultFlag = "complete";
        }

    }

    if ( fileExistFlag != "false" )
    {
        outFile << std::endl;
        outFile.close( );
    }

    vec.clear( );


    return resultFlag;
}

bool doesFileExist( std::string article )
{
    return boost::filesystem::exists( getPath( article ) );
}

std::string getPath( std::string article )
{
    return "./Articles/" + article;
}

std::string addWikiLink( std::string article )
{
    std::string test = "\"https://en.wikipedia.org/wiki/" + article + "\"";

    return test;
}
