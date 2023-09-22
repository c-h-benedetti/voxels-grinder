
### NOTE:
Avec cette implémentation, on peut librement créer des conteneurs à utiliser pendant nos calculs avec notre librairie et qu'ils soient complètement détachés de la gestion d'instances de VG.
Cela va être très utile pour les opérations qui nécessitent des opérations intermédiaires.
Les opérations auront lieu sur des classes filles à partir de Data. Le résultat sera registered auprès de VG, ce qui transfèrera l'ownership.


### NOTE:
On voudra avoir un menu "Segmentation" dont les items seront des noms d'objets ("yeasts", "blobs", "astrocytes", ...) qui seront des workflows
prédéfinis dont l'utilisateur peut tuner les settings et lancer tel quel sur son image.
Ces workflows seront classés avec des arbres sémantiques et des tags.
Il sera possible de les mettre en commun sur un serveur, ou de faire en sorte que certains dépôts soient publics.


### NOTE:
Pour le calcul des voisins, on peut réduire le nombre d'évaluations du if en calculant les voisins sous forme de liste avant le calcul lui-même.
On lance le calcul des voisins sur chaque bordure (coûteux), puis pour le reste de l'image, on peut faire une boucle sans check.

### NOTE:
Le site de VG doit proposer un catalogue de scripts. Le responsable du script doit pouvoir éditer les tags (ontologie) de son script. Un script n'est pas obligatoirement hosted sur le site de VG, cela peut simplement être un lien vers une release ou une branche d'un github. La barre de recherche doit aussi être capable d'aller chercher dans cette base de données (qui n'est fetch qu'au démarrage de VG).
Les scripts doivent être référencés sur le site. On doit pouvoir leur donner un identifieur, un wiki, ...
Tout le monde peut upload, il faudrait quand même un moyen de contrôle automatique. Il faudra donc que les utilisateurs fournissent des images qui seront utilisées pour tester le plugin, ainsi que des tests unitaires. La structure devra être simpliste et le soft doit être capable de créer un nouveau projet. Le testeur testera sur tous les OS que l'outil se lance, qu'il ne crash pas quand on l'utilise avec les tests unitaires, que les icônes sont valides, ...

### NOTE:
Pour pouvoir facilement retrouver des éléments par la recherche, il faudra implémenter un système d'ontologie pour les plugins. Une connexion directe à la base de données Biii sera également un plus.

### NOTE:
Dans la documentation, il doit être possible de voir en coup d'oeil (code couleur ?) si une fonction est destructive ou non.

### NOTE:
**Idées de noms**:
- Cevenole
- Divination (divination.bio)

### TODO:
- [ ] Passer les images en `std::unique_ptr` ou `std::shared_ptr` pour qu'on puisse chainer les appels.
- [ ] Dans chaque Image, ajouter une `std::pair` qui représente la range de valeurs.
- [X] Ajouter les tailles et autres attributs.
- [ ] Ajouter le vector de bounding boxes pour les images. Comment est-il utilisé pour le processing.
- [ ] Faire des fonctions qui génèrent des mock images pour les tests.
- [ ] Essayer de détecter le bon type avec le dtype de l'image.
- [ ] Intégrer un arbre sémantique aux descriptifs de plugins (des tags).
- [ ] Lier les opérations et les buckets. Comment facilement utiliser les buckets avec les threads. Il faudra certainement passer une fonction aux threads.
- [ ] En plus des outils de base, le soft doit proposer des morceaux de workflow et des workflow.
- [ ] Il doit être possible de créer un workflow en quelques clics.
- [ ] Ajouter un type `Mask` qui hérite de `Image8`.
- [ ] Ajouter un type `Labeling` qui est sur 64 bits.
- [ ] Changer l'implémentation pour qu'Image soit toujours la classe mère polymorphe mais que les ImageN héritent d'un template.
- [ ] Faire l'algo pour qu'une opération utilise les workers.
- [ ] Écrire les tests unitaires de sorte à ce qu'ils soient la documentation du code.
- [ ] Ajouter un équivalent de `regionprops` qui prend une image labélisée et une collection d'images normales et permet de faire de sortir des stats.


### NOTE: (Levels)
**(0)·** Fonctions et objets qui sont, ou agissent directement sur les données (Image, Mesh, IO, ...). Tout ce qui ferait une librairie d'image processing.  
**(1)·** Fonctions et objets qui caractérisent le fonctionnement du logiciel en mode 'prompt'/'ligne de commande' (Media, MediaManager, ...).  
**(2)·** Éléments relatifs à l'embedding de Python.  
**(3)·** Éléments relatifs au fonctionnement interne des nodes (sans GUI).  
**(4)·** Éléments relatifs à la GUI et à l'interfaçage entre cette dernière et les fonctions de VG.  

---

#### VERSION: (0.0.1a)
- [ ] Load et save un tiff complet (channels, slices et frames. Uniquement les données d'image).
- [ ] Appliquer des opérations de base (threshold, split channels, qqs opération de morphologie & projection).
- [ ] Écrire du texte sur une image.
- [ ] Load et save un mesh (vertices, faces & normales).
- [ ] Appliquer des opérations de base sur le mesh (scale le long des normales, smoothing, centrer, rotate).

Dans cette version, on s'en tient à des éléments de niveau 0.

---

#### VERSION: (0.0.1b)
- [ ] Mécanisme de sélections prêt à l'emploi.

---

#### VERSION: (0.0.1c)
- [ ] Opérations plus coûteuses avec le threads-pool (côté CPU).
- [ ] Opérations plus complexes comme le watershed et le marker-based watershed (avec une points selection).
- [ ] Utiliser les différents threads pour faire des stats (min, max, avg, ...).
- [ ] Implémenter le 'regionprops'.

 ---

 ### VERSION: (0.0.1d)
- [ ] Tenter d'ajouter CUDA pour le calcul distribué.
- [ ] Support du format OME-ZAR.

---

#### VERSION: (0.0.2)
- [ ] Ajouter le scripting Python.
- [ ] Faire tout le refactoring que ça implique.

---

#### VERSION: (0.0.3)
- [ ] Ajouter le support de nodes sans interface graphique.
- [ ] Faire tout le refactoring que ça implique.

---