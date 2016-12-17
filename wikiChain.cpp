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


void getInput( std::string & , std::string & ); // Get user input ( starting and ending word )
std::string fixSpaces( std::string );           // Fixes spaces
std::string addWikiLink( std::string );         // Adds the wikiepedia page link to the string

void callPythonScript( std::string );           // Calls a python script that gets all hyperlinks from within a page
                                                // That same script will place all those links into a text file
                                                // The text file will be titled after the title of the wikipedia page
                                                // The contents of the file will be the hyperlinks within the page

std::string getArticles( std::string, std::list< std::vector < std::string > > &, int &, bool );
std::vector< std::string > getLinks( int &, bool, std::string );
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

    bool fileExist               = false;

    std::ofstream outFile;
    std::ofstream resetFile;

    //resetFile.open( "Data.txt", std::ofstream::trunc );
    //resetFile.close( );

    int count = 0;

    //outFile.open( "Articles.txt", std::ofstream::trunc );

    // startFlag and endFlag are returned by reference
    getInput( startFlag, endFlag );

    //  If the starting article does not exist
    //  call the python script
    //  else, it already exist so do not call the script
    if ( !doesFileExist( startFlag ) )
        callPythonScript( startFlag );

    else
        fileExist = true;

    //std::cout << fileExist << std::endl;

    tmp.push_back( startFlag );
    listQueuArticles.push_front( tmp );

    //outFile << startFlag << std::endl;
    std::cout << startFlag << std::endl;

    // This while loop is responsible for the majority of the work
    while ( completeFlag != "complete" || listQueuArticles.empty( ) )  //   May have to check the 2nd parameter here
    {
        completeFlag = getArticles( endFlag, listQueuArticles, count, fileExist );

        if ( !listQueuArticles.empty( ) )
            listQueuArticles.pop_front( );

        if ( completeFlag != "complete" )
        {
            std::string currentArticle = listQueuArticles.front( ).back( );

            // Name of the article that is currently being visited
            std::cout << currentArticle;

            if ( !doesFileExist( currentArticle ) )
            {
                fileExist = false;
                std::cout << std::endl;
                callPythonScript( currentArticle );
            }

            else
            {
                fileExist = true;
                std::cout << " - Not calling python script - file exist" << std::endl;
            }
        }
    }

    std::cout << "\nLinks visited: " << count << std::endl;
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
    std::string articleName;

    wikiLink = addWikiLink( article );
    articleName = " \"" + article + "\"";

    std::string command = "python linkFetcher.py ";

    command += wikiLink;
    command += articleName;

    system( command.c_str( ) );

}

std::string getArticles( std::string endArticle, std::list< std::vector< std::string> > & articles, int & i, bool fileExist )
{
    std::ofstream outFile;
    std::string line;
    std::string title;

    std::vector< std::string> vec;
    std::string quit;

    vec = getLinks( i, fileExist, articles.front( ).back( ) );

    std::string status = scanArticle( vec, articles, endArticle );

    if ( status == "complete")
        return status;

    //outFile.open( "Data.txt", std::ios_base::app );

    //outFile << "\n" << changeMe << std::endl;

    vec.clear( );
    return "no_match";
}


std::vector< std::string > getLinks( int &i, bool FileExist, std::string article )
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

    // So im just wasting time, why not just let the pyhton script create the file???
    // that means the cutting of unnecessary parts should be done by the pyhton script
    // Recreating the files is just wasting time and processing power
    // The file creation part should be left to python since it already creates the files from the beginning
    // TO-DO: All of the following parts in this function should be done in python not in C++
    // That means cut off ":" and "Help" and only keep the Category.
    // Also only keep clean english and not the weird characters. So something from line 215
    // Also delete the last two lines since it seems to be the name of the article
    // Priority - HIGH


    myFile.open( getPath( article ).c_str( ) );

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
        //outFile.open( fileName, std::ios_base::app );
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

        //if ( fileExistFlag == "false")
        //outFile << vec[i] << std::endl;

        if ( endArticle == vec[i] )
        {
            //std::cout << "\nMATCH" << std::endl;
            resultFlag = "complete";
        }

    }

    if ( fileExistFlag != "false" )
    {
        //outFile << std::endl;
        //outFile.close( );
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
