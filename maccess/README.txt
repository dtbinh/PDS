Alexandre Moevi - Marion Tommasi
TP1 PdS 26/09/2014

Fonctionnement :
"make" ou "make all" permet de créer les trois exécutables, prlimit, maccess et maccess+.
"make test" permet de lancer plusieurs commandes affichant différentes erreurs liées au maccess+.
"make clean" permet de nettoyer le répertoire.

"./maccess" prend en argument d'abord un fichier puis l'une des trois options (-r, -x ou -w) et éventuellement l'option -v. Cette fonction vérifie les droits d'accès de lecture, d'exécution ou d'écriture du fichier en paramètre. L'option -v affiche l'erreur le cas échéant.

"./maccess+" prend en premier argument le fichier puis les options -rxw dans n'importe quel ordre, l'option -v affiche l'erreur du test du droit précédent

Ex: ./maccess+ file -rv -wxv
    v ne s'applique que pour le 'r' (lecture) et le 'x' (exécution). 


Commentaires :
Nous n'avons pas réussi à enregistrer une session shell dans un fichier texte, ni n'avons fait de script shell.


