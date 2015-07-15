# SY5 - PROJET SYSTEMES

Un simulateur d'un système d'impressions

## Compilation

    make

## Pour l'execution des programmes

    ./print_demon -t tubeServeur -c fichierConfiguration

Pour la requête simple d'impression :

    ./init_simulateurs -c fichierConfiguration

Pour la requête simple d'impression :

    ./mpr -P nomImprimante fichier tubeClient

Pour annulation impression :

    ./mpr -P nomImprimante fichier tubeClient -A idImpression

Pour envoyer N impression du fichier "fichier" :

    ./mpr -P nomImprimante fichier tubeClient -N nombre

Imprime chaque fichier specifié dans listeFichiers :

    ./mpr -P nomImprimante fichier tubeClient -M listeFichiers
