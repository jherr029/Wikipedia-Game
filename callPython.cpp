#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>


#include<list>
#include<stack>

int main( )
{

    std::string completeFlag = "incomplete";

    std::string fileName = "testing";
    std::string endArticle = "Pattern";

    std::string command = "python test.py ";

    std::cout << "Not Done " << std::endl;

    command += fileName;

    system( command.c_str( ) );
    
    std::cout << "Done" << std::endl;

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
                break;
            }
            std::cout << line << std::endl;
        }
    }

    myFile.close( );

    std::cout << "Size: " << listOfTitles.size( ) << std::endl;
    std::cout << "Front: " << listOfTitles.front( ) << std::endl;
    std::cout << "Back: " << listOfTitles.back( ) << std::endl;

    return 0;
}
