import urllib
import lxml.html
import sys
import os.path

#print sys.argv[1]

connection = urllib.urlopen( sys.argv[1] )
#connection = urllib.urlopen( "https://en.wikipedia.org/wiki/Muzaffar_Ahmed_(politician)")

dom = lxml.html.fromstring( connection.read( ) )



banned1 = "PubMed_Identifier"
banned2 = "Digital_object_identifier"
banned3 = "International_Standard_Serial_Number"
banned4 = "PubMed_Central"
banned5 = "International_Standard_Book_Number"
banned6 = "Bibcode"
banned7 = "Portal"
banned8 = "Help"
banned  = "Category"


s = "./Articles/"
tmp = sys.argv[2]
tmp = tmp[0: ]
path = s + tmp

#print path
#print "Opening the file..."
if not os.path.isfile( tmp ):
    target = open( path, 'w')

    target.truncate( )

    for link in dom.xpath('//a/@href'): # select the url in href for all a tags(links)
        if link.startswith( '/wiki/'):
            article = link[6: ]
            if not article.startswith(banned) and not article.startswith( 'File' ) and not article.startswith('Special') and\
               not article.startswith('Wikipedia') and not article.startswith('Template'):
                if not article.startswith(banned1) and not article.startswith(banned2) and not article.startswith(banned3) and \
                   not article.startswith(banned4) and not article.startswith(banned5) and not article.startswith(banned6) and \
                   not article.startswith(banned7) and not article.startswith(banned8):
                    path = s + article
                    target.write( article )
                    target.write( "\n")
            #print article

#print "Closing file"
target.close( )
