### INSTALLATION

- Télécharger igraph et l'installer:

Sur un debian:

`wget http://igraph.org/nightly/get/c/igraph-0.7.1.tar.gz̀`   
`tar -xvzf igraph-0.7.1.tar.gz`   
`cd igraph-0.7.1`   
`sudo apt-get install build-essential libxml2-dev libsfml-dev`   
`./configure`   
`make`   
`make check`   
`sudo make install`   
`cd ..`   
`rm -rf igraph-0.7.1 igraph-0.7.1.tar.gz`   

- Trouver vos dossiers d'installation du .h et de la bibliothèque .so :

`pkg-config --libs --cflags igraph`   
(flags à ajouter à vos lignes de commande g++, modifiez le Makefile)

- Compiler le programme:

`make`   


### UTILISATION
