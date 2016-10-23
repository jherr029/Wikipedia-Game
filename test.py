import urllib
import lxml.html
import sys


print sys.argv[1]

connection = urllib.urlopen( sys.argv[1] )

dom = lxml.html.fromstring( connection.read( ) )


print "Opening the file..."
target = open( "myFile", 'w')

target.truncate( )


for link in dom.xpath('//a/@href'): # select the url in href for all a tags(links)
    if link.startswith( '/wiki/'):
        target.write( link[6: ] )
        target.write( "\n")
        #print link

print "Give me the input"

#line1 = raw_input("line 1: ")

#print "writing to the file"

print "Closing file"
target.close( )
