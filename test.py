import urllib
import lxml.html
import sys


print sys.argv[1]

connection = urllib.urlopen( sys.argv[1] )
#connection = urllib.urlopen( "https://en.wikipedia.org/wiki/Muzaffar_Ahmed_(politician)")

dom = lxml.html.fromstring( connection.read( ) )


#print "Opening the file..."
target = open( "myFile", 'w')

target.truncate( )


for link in dom.xpath('//a/@href'): # select the url in href for all a tags(links)
    if link.startswith( '/wiki/'):
        target.write( link[6: ] )
        target.write( "\n")
        #print link

#print "Closing file"
target.close( )
