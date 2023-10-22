#ifndef CANVAS_STATISTICS_HPP_INCLUDED
#define CANVAS_STATISTICS_HPP_INCLUDED

#include "Data.hpp"
#include "Bucket.hpp"
#include "Task.hpp"
#include "Processor.hpp"

class CanvasStatistics : public Processor {
    Data* target;

};

#endif // CANVAS_STATISTICS_HPP_INCLUDED

/** TODO:
*
* - [ ] Les statistiques sont calculées pour toutes les slices du channel et de la frame 'en cours', représentée par des indices en argument.
* - [ ] On peut passer une liste d'indices pour pouvoir calculer les stats de plusieurs channels et plusieurs frames.
* - [ ] Quelque soit le mode, l'affichage en cours est toujours pour la frame courante.
*
*/

/** NOTES:
*
* - En mode slice par slice, on a un buffer séparé pour chaque slice.
* - En mode full-stack, on garde ce fonctionnement mais on fait l'addition dans une méthode (ou un buffer supplémentaire)
* - Il faut un moyen d'indiquer si on veut avoir plusieurs channels sur l'histogramme.
*
*/

/** FEATURES:
*
* - [ ] Stats + histo de ce qui est à l'écran (current slice-channel-frame).
* - [ ] Stats + histo d'un stack entier.
* - [ ] Stats + histo à l'intérieur d'une sélection ou plusieurs sélections donnée(s).
* - [ ] Mode live où on peut voir l'histo changer selon la slice, le channel, ...
* - [ ] Mode où on peut voir les histogrammes superposés de tous les channels.
* - [ ] En même temps, on veut avoir les autres stats (min, max, mean, median, stddev, ...)
*
*/