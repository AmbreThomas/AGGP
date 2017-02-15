# AGGP

== INSTALLATION == 

- Télécharger igraph et l'installer:

Sur un debian:

wget http://igraph.org/nightly/get/c/igraph-0.7.1.tar.gz
tar -xvzf igraph-0.7.1.tar.gz
cd igraph-0.7.1
sudo apt-get install build-essential libxml2-dev
./configure
make
make check
sudo make install
cd ..
rm -rf igraph-0.7.1 igraph-0.7.1.tar.gz

- Trouver vos dossiers d'installation du .h et de la bibliothèque .so :

pkg-config --libs --cflags igraph
(flags à ajouter à vos lignes de commande g++)

puis ajoutez le dossier de la bilbio à votre path pour pouvoir exécuter le programme:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<chemin du dossier>

par exemple:
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

- Compiler le programme:

avec les flags trouvés avec la commande pkg-config précédente, par exemple:

g++ main.cpp -I/usr/local/include/igraph -L/usr/local/lib -ligraph -o res_generator

ou une fois qu'on aura un Makefile propre:

make
