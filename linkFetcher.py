import urllib
import lxml.html
import sys
import os.path

#print sys.argv[1]

connection = urllib.urlopen( sys.argv[1] )
#connection = urllib.urlopen( "https://en.wikipedia.org/wiki/Muzaffar_Ahmed_(politician)")

dom = lxml.html.fromstring( connection.read( ) )


#print "Opening the file..."
target = open( "myFile", 'w')

target.truncate( )

s = "./Articles/"

for link in dom.xpath('//a/@href'): # select the url in href for all a tags(links)
    if link.startswith( '/wiki/'):
        article = link[6: ]
        path = s + article
        #print path

        if not os.path.exists( path ):
            target.write( article )
            target.write( "\n")
            #print article

#print "Closing file"
target.close( )
