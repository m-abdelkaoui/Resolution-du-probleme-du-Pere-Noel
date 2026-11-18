Problème du Père Noël — Optimisation par recuit simulé

Présentation du projet:

Ce projet consiste à résoudre un problème d'optimisation combinatoire portant sur la répartition de cadeaux entre plusieurs enfants.

Chaque enfant possède un niveau de satisfaction différent pour chaque cadeau. L'objectif est de trouver une répartition qui maximise la satisfaction de l'enfant le moins satisfait.

Le problème est soumis à une contrainte : chaque cadeau ne peut être attribué qu'à un seul enfant.

Pour résoudre ce problème, nous avons conçu et implémenté une approche basée sur l'algorithme du recuit simulé, une méthode d'optimisation permettant d'explorer différentes solutions afin de rechercher une répartition plus équitable.

Objectif:

Maximiser la satisfaction minimale parmi l'ensemble des enfants.

Autrement dit, nous cherchons à améliorer la situation de l'enfant le moins satisfait, plutôt que de simplement maximiser la satisfaction totale.

Les principales contraintes sont :
- Chaque cadeau doit être attribué une seule fois.
- Un enfant peut recevoir plusieurs cadeaux.
- La satisfaction dépend des préférences individuelles de chaque enfant.
- La répartition doit respecter les contraintes du problème.

Approche algorithmique

1. Initialisation

Une première répartition des cadeaux est créée. Les cadeaux sont distribués successivement entre les enfants afin d'obtenir une solution de départ.

2. Calcul de la satisfaction

Pour chaque enfant, nous calculons une satisfaction normalisée :

La satisfaction obtenue avec les cadeaux reçus est rapportée à la somme des satisfactions possibles pour l'ensemble des cadeaux.

Cette normalisation permet de comparer les niveaux de satisfaction des différents enfants.

3. Fonction objectif

L'énergie du système correspond à la satisfaction normalisée minimale parmi tous les enfants.

L'objectif de l'algorithme est donc de maximiser cette valeur.

4. Recuit simulé

Nous utilisons le recuit simulé pour explorer progressivement de nouvelles répartitions.

Deux types de transitions sont implémentés :

->Transfert : un cadeau est retiré à un enfant et attribué à un autre.
->Échange : deux enfants échangent aléatoirement un de leurs cadeaux.

À chaque itération :
1. Une nouvelle solution est générée.
2. Sa satisfaction minimale est calculée.
3. La différence avec la solution précédente est évaluée.
4. La nouvelle solution est acceptée selon un critère probabiliste dépendant de la température.

La température diminue progressivement afin de limiter l'acceptation de solutions moins intéressantes au fil des itérations.

Technologies utilisées

- Langage : C
- Bibliothèques : stdio.h, stdlib.h, math.h, time.h
- Optimisation : Recuit simulé
- Structures de données : Tableaux et structures
- Compilation : GCC


Compilation et exécution

Sous Linux, avec GCC :


gcc main.c -o programme -lm


Exécution :

./programme


Le programme charge les données depuis `information.txt`, génère une répartition initiale, applique le recuit simulé et affiche les résultats avant et après optimisation.

 Résultats

Le programme affiche notamment :

- Le nombre de cadeaux reçus par chaque enfant.
- La satisfaction normalisée de chaque enfant.
- La satisfaction minimale globale avant optimisation.
- La satisfaction minimale globale après optimisation.
- Le temps d'exécution CPU.

 Compétences développées

- Conception et analyse d'une solution algorithmique.
- Implémentation d'un algorithme d'optimisation en C.
- Manipulation de structures et de tableaux.
- Gestion de contraintes dans un problème combinatoire.
- Utilisation du recuit simulé.
- Analyse des performances et du temps d'exécution.

