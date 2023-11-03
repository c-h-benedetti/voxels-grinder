#ifndef CANVAS_STATISTICS_HPP_INCLUDED
#define CANVAS_STATISTICS_HPP_INCLUDED

#include <vector>
#include <map>
#include <utility>
#include "Data.hpp"
#include "Bucket.hpp"
#include "Task.hpp"
#include "Processor.hpp"

struct Statistics {
    std::vector<uint32_t> histogram;
    
    float min, max;
    float mean, stddev;
    float q1, median, q3;
    float binSize;

    Statistics();
};

class CanvasStatistics : public Processor {
    
    Data* target;
    /**
     * The pair contains the index of the channel and the frame.
     * The vector contains a Statistics instance for each individual slice.
     * So we can have the stats for a slice from a channel from a frame.
     */
    std::map<std::pair<size_t, size_t>, std::vector<Statistics>> statistics;

private:

    /**
     * Task is charge of processing the stats for a slice from a channel from a frame.
     */
    struct ProcessStatsTask : public Task {
        std::vector<Statistics> stats; // Must be as long as there are slices.
        Data* target;

        int run(const MaskCanvas*, Bucket b) override;
        int run(const LabeledCanvas*, Bucket b) override;
        int run(const VoxelsCanvasU8*, Bucket b) override;
        int run(const VoxelsCanvasU16*, Bucket b) override;
        int run(const VoxelsCanvasFloat*, Bucket b) override;
        int run(const VoxelsCanvasTriplet*, Bucket b) override;

        int execute(Bucket b) override;

        ProcessStatsTask() = delete;
        ProcessStatsTask(Data* t);
    };

    std::vector<Bucket> make_buckets();
    void set_target(Data* t);

public:

    inline bool is_streamable() const override { return true; }

    void process_statistics(size_t channel=0, size_t frame=0);
    bool stats_available(size_t channel=0, size_t frame=0);
    const std::vector<Statistics>& get_stats_by_slice(size_t channel=0, size_t frame=0);
    Statistics get_canvas_stats(size_t channel=0, size_t frame=0);

    CanvasStatistics() = delete;
    CanvasStatistics(Data* t);

};

#endif // CANVAS_STATISTICS_HPP_INCLUDED

/** NOTES:
 *
 * Dans la mesure où nous travaillons sur des types différents, on ne peut pas utiliser une unordered_map pour le stockage car les clés doivent être du type de la donnée.
 * De plus, comment accéder aux éléments d'une image float ? En indexant avec des floats ?
 * Pour cela, on est obligés de garder un vector pour compter. En revanche, il faudra jouer avec les bins pour les types les plus grands.
 * Il faut un moyen de déterminer automatiquement le binnig à appliquer s'il n'est pas spécifié par l'utilisateur.
 * Cependant, il faut conserver un invariant.
 * On ne peut pas systématiquement donner un histo de taille 256 ou 65536 en cas de ces types et autre chose pour le reste.
 * L'accès des histogrammes doit être le même pour tout le monde.
 * Les histogrammes commencent toujours pour la valeur min et se terminent par la valeur max (inclues)
 * Pour les types basiques, on peut forcer un binning de taille 1.
 *
 * Images must have accessors:
 *   operator[] permet de faire un accès par index dans le tableau linéaire des données.
 *   operation() doit pouvoir prendre toutes les coordonnées sur chaque axe et les utiliser au besoin.
 *
 * Comment changer l'implémentation pour les images "virtuelles" ? En effet, les slices peuvent ne pas être disponibles entièrement!
 * Il ne faut pas sortir des limites du bucket. Chaque bucket doit savoir à qui se référer, et les réceptacles doivent exister avant de commencer.
 * Au début d'un nouveau thread, on récupère ce qui a déjà été fait jusque là.
 *
 * Pour les images "virtuelles", les buckets de calcul doivent être adaptés.
 * Il faut une méthode dans les tasks qui renvoie true ou false selon si l'implémentation pour les images virtuelles est dispo.
 * Pareil pour les processors.
 *
**/
