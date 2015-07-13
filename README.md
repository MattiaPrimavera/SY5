# SY5 - PROJET SYSTEMES
Un simulateur d'un système d'impressions


Compilation: commande make


Pour l'execution des programmes : 

./print_demon -t tubeServeur -c fichierConfiguration
./init_simulateurs -c fichierConfiguration
./mpr -P nomImprimante fichier tubeClient (pour la requete simple d'impression)
./mpr -P nomImprimante fichier tubeClient -A idImpression (pour annullation impression)
./mpr -P nomImprimante fichier tubeClient -N nombre (pour envoyer N impression du fichier "fichier")
./mpr -P nomImprimante fichier tubeClient -M listeFiciher (imprime chaque fichier specifie' dans listeFichier)
