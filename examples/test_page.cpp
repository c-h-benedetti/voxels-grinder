
/**

 \page rootpage À PROPOS DU PROJET

 ## Media weights examples

 | nFrames  | nSlices  | nChannels | Width    | Height   | Bit depth  | Poids |
 |----------|----------|-----------|----------|----------|------------|-------|
 |   10     |   30     |    5      |  2048    |  2048    | 32 (float) | 25GB  |
 |    1     |   10     |    4      |  2048    |  2048    | 32 (float) | 0.8GB |
 
 ---

 ## 📌 Introduction

 Cette page sert à avoir une overview de ce qu'il se passe dans le projet actuellement.<br>
 Pour parler plus précisement de l'implémentation de chaque classe, ou de la synergie entre plusieurs classes, il est plus adapté de faire une sous-page.<br>
 Montrez autant d'exemples que possible avec des explication ultra-détaillées. Ces pages ne seront pas lues que par des développeurs.<br>
 Par exemple, les premières pages qui devraient être créées sont celles pour expliquer le fonctionnement des segments du stack, et surtout le fonctionnement des actions.
 
 ---

 ## 📌 TODO

 ### Global

 - ◯ Expliquer le langage de macro va être remplacé par du scripting nodal.
 - ◯ Transférer tous les todo dispersés dans le code vers ces pages.
 - ◯ Retirer le MediaManager et transférer ses fonctions dans Media.

 ### MediaManager
 
 - ◯ Une méthode pour créer un Media à partir d'un objet MediaData.
 - ◯ Ajouter la génération des blocs de data dans les méthodes `MediaManager::createMedia()`.
 
 ### MediaData

 - ⬤ Des fonctions pour pouvoir dupliquer des MediaData de tous types.
 - ◯ Passer tous les constructeurs en `private`.
 - ◯ Inverser les positions de slice et channel dans les fonctions.
 - ◯ Faut-il une sécurité pour s'assurer qu'un objet de MediaData est possédé par au plus un Media ?
 - ◯ En ce qui concerne les accesseurs, il y en aura 2 types distincts. Le premier permettra d'accéder directement aux valeurs pixel par pixel en donnant tous les indices requis. Le second sera spécifiquement pour accéder à une image et renvera une std::pair qui donnent l'index de début et de fin d'une image.
 - 🛠️ Les fonctions d'accès par indice devraient avoir un objet associé qui permet de ne pas refaire les calculs à chaque fois, ça pourra sauver beaucoup de temps passé à faire des multiplications identiques à chaque itération.
 - 🛡️ La création d'un objet de `RGBDataArena` doit sécuriser le nombre de channels fixé à 3.
 - 🛡️ Rajouter des protections pour jeter des erreurs si les dimensions sont invalides (0 sur une dimension ou autre).
 - 🛠️ Corriger l'indexation des images RGB dans lesquelles les 3 channels sont entrelacés.

 ### Media

 - ◯ Une méthode `duplicate()` qui construit une nouvelle instance basée sur l'appelante.
 - ◯ Check que le nom d'un Media n'existe pas déjà lors des instanciations et des renamings. La fonction retourne le nom qui a été donné.

 ### DataUtils

 - ◯ Les fonctions de conversion d'un type à l'autre. La fonction de conversion depuis du RGB doit laisser le choix de l'algo de transformation en gris (soit un channel au choix, soit la formule).

 ### DataProcessor

 - ◯ Des fonctions pour normaliser ou recaler toutes les valeurs dans une certaine range.
 - ◯ Equalizer d'histogramme.
 - ◯ Threshold + méthodes automatique de calcul + celle utilisée dans ComDet.

 ### Dimensions

 - 🧨 Dans les boucles imbriquées, les indices générés par `indexOf()` doivent être consécutifs.

 ### Autre

 - ◯ Pour `doxygen-awesome` il faut un mode qui permet de signaler qu'une tâche est terminée. Pour ça, il faut que le texte soit passé en opacité 50%, en italique et rayé.

 ---

 ## 📌 Plugin à intégrer (ordre == priorité)

 - ◯ MorphoLibJ
 - ◯ FeatureJ
 - ◯ LabKit
 - ◯ ComDet
 - ◯ TrackMate

 ---

 ## 📌 Implémentation

 \subpage actionspage <br>
 \subpage mediasmanagementpage <br>
 \subpage nodegraphpage <br>
 \subpage exemplepage

*/





/**
 \page mediasmanagementpage Gestion des médias
 
 ## 📌 Principes

 - Les seuls objets affichables (GUI) sont les instances de Media. Ces structures sont donc gérées par la classe MediaManager qui est un singleton à l'échelle d'une instance d'exécution. Les instances de Medias sont impossibles à copier et la méthode `close()` de ces objets permet d'informer l'instance de MediaManager. 
 - L'instance de MediaManager peut être appelée depuis n'importe où avec la méthode `MediaManager::getInstance()`. Cette fonction renvoie l'objet à coup sûr. Des méthodes permettent d'accéder à des instances de Media soit par ID, soit par titre. Les méthodes d'accès renvoient des raw pointers pour pouvoir retourner nullptr en cas de non-existance. On ne veut pas utiliser de shared_ptr car la propriété de l'objet ne doit pas être partagée, ni de unique_ptr car la propriété ne doit pas être transférée.

 ---

 ## 💡 À expliquer
 
 - ◯ Un Media affichable pour l'utilisateur ne peut être généré que par la classe MediaManager.
 - ◯ Conversions d'un type à l'autre.
 - ◯ Les images sont regroupées par channel plutôt que par stack, cela donne lieu à une indexation différente de dans ImageJ. Pour l'utilisateur ça ne change rien. Pour le scripting ou le développement, il faut être plus vigilant.
 - ◯ La classe de base MediaData ne permet pas d'accéder aux données stockées, il faut obligatoirement caster l'objet vers sont type dynamique pour cela.
 - ◯ La map est obligée de stocker des pointeurs de Media car le destructeur de Media est private (et une map ne peut pas stocker des objets dont le constructeur ne peut pas être appelé). Pour contourner ce problème, les std::unique_ptr sont utilisés avec la possibilité de leur passer une fonction de destruction personnalisée.
 
 ---

 ## 📑 Notes

 - C'est la classe de processing de base qui contiendra les fonctions pour convertir d'un type à l'autre tout en appliquant une transformation.

 ---

 ### Indexation dans les data

 Dans les opérations coûteuses (comme le calcul) il est rare qu'on ait besoin d'accéder à tous les channels d'une même image en même temps. Il est plus courrant de devoir accéder aux différentes slices d'un même channel en même temps. Dans cette implémentation, les données sont regroupées par channel plutôt que par stack: toutes les slices d'un même channel sont contigues.<br>
 La formule pour accéder à un voxel précis est donc `v = (f*nSlices*nChannels*K) + (ch*nSlices*K) + (sl*K) + (r*width) + c` dans laquell `K` désigne le résultat de `width*height`.<br>
 ⚠️ Ce mode d'indexation n'est pas vrai pour le type RGBData, dont les valeurs des 3 channels sont entrelacés telles qu'elles le sont dans la plupart des formats de stockage d'images.

 ### Principes

 - Le MediaManager est un singleton à l'échelle d'une instance d'exécution du logiciel. Cet objet possède toutes les instances d'objets Media. Un objet Media ne peut pas être affiché ou géré par le logiciel s'il n'est pas possédé par l'instance de MediaManager.
 - Beaucoup de méthodes, dont le destructeur, sont privées dans Media. On veut aussi pouvoir déplacer les instances si besoin. C'est pour cela que la map contient des unique_ptr de Media, plutôt que des Medias simples. On peut ainsi utiliser le destructeur personnalisé du pointeur.
 - Une instance de Media possède un objet de type ByteData, ShortData, FloatData ou RGBData. Ces objets sont à instancier depuis les fonctions `DataUtils::createXXXData()` car ces instances n'ont pas à être associées à un Media pour exister. Par exemple, les fonctions de calculs retourneront souvent, ou prendront souvent en argument des MediaData plutôt que des Media eux-mêmes.
 - Les blocs de data peuvent être castés d'un type vers l'autre avec les fonctions appropriées dans DataUtils.

 */





/**
 \page nodegraphpage Scripting avec des nodes
 
 ## 💡 À expliquer

 ---

 ## 📑 Notes

 */





/**
 \page actionspage Actions & ActionManager
 
 ## 💡 À expliquer
 
 - ◯ L'emplacement des actions est défini par son chemin (`xxx.yyy.zzz`). Il est donc possible de choisir précisement où placer l'action, contrairement à ImageJ où tout est entassé dans le menu "Plugins". Il est même possible de rajouter une action dans un menu vanilla de l'application.
 - ◯ Les paramètres sont ultra-détaillés pour pouvoir venir de n'importe où.
 - ◯ Le système d'auto-registering des actions à la compilation qui dans sa version actuelle recquiert une spécificité dans le CMake.
 - ◯ Le parsing des arguments reçu dans une collection de types génériques qu'on peut accéder avec des IDs.
 
 ---

 ## 📑 Notes

 - En soit il ne sera pas utile de faire des constructeurs d'actions qui prennent directement les paramètres car les vrais fonctions pourront être appelées depuis le code. Cependant, si certaines fonctions ou suites de fonctions sont exclusives à une action, il serait mieux de rendre l'objet facilement constructible.

 */





/**
 \page exemplepage Exemple de snippets
 
 ## Exemple de test unitaire

 ### Première partie
 
 Le test unitaire suivant vérifie qu'une instance de MediaManager est capable de contenir des instances de Media en conservant les bons identifiers.
 
 - ◯ Tâche à faire.
 - ⨂ Tâche annulée.
 - ⬤ Tâche terminée.
 - 🛡️ Sécurisation/bug fix.
 - 🛠️ Optimisation.
 - 🦺 Refactoring.
 - 🧨 Test unitaire.
 
 \snippet medias/main.cpp Ca c'est l'ID utilisé

 ### Deuxième partie

 On peut aussi utiliser du code `inline` avec *MarkDown*.
 */