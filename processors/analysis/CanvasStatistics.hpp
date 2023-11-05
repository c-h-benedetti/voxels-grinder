#ifndef CANVAS_STATISTICS_HPP_INCLUDED
#define CANVAS_STATISTICS_HPP_INCLUDED

#include <vector>
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
    std::vector<Statistics> statistics; // We make the statistics of the current stack. One index in the vector is the index of a sliee.
    Bucket dimensions;

private:

    struct BinningPropertiesTask : public Task {
        Data* target;
        std::pair<float, float> extremas;
        float binSize = 1.0f;
        size_t nBins;

        int run(const MaskCanvas*, Bucket b) override;
        int run(const LabeledCanvas*, Bucket b) override;

        int run(const VoxelsCanvasU8*, Bucket b) override;
        int run(const VoxelsCanvasU16*, Bucket b) override;
        int run(const VoxelsCanvasFloat*, Bucket b) override;

        int execute(Bucket b) override;

        BinningPropertiesTask() = delete;
        BinningPropertiesTask(Data* target, std::pair<float, float> e);
    };

    struct MinMaxSliceTask : public Task {
        
        Data* target;
        std::vector<Statistics>& slices;
        Bucket dimensions;
        size_t channel;
        size_t frame;

        int run(const MaskCanvas*, Bucket b) override;
        int run(const LabeledCanvas*, Bucket b) override;

        int run(const VoxelsCanvasU8*, Bucket b) override;
        int run(const VoxelsCanvasU16*, Bucket b) override;
        int run(const VoxelsCanvasFloat*, Bucket b) override;

        int execute(Bucket b) override;

        MinMaxSliceTask() = delete;
        MinMaxSliceTask(Data* target, Bucket d, std::vector<Statistics>& s, size_t c, size_t f);
    };

    struct HistogramSliceTask : public Task {
        
        Data* target; // Canvas from which we have to extract histograms
        std::vector<Statistics>& slices; // Statistics containers
        Bucket dimensions; // Global dimensions of the canvas
        size_t channel; // Channel from which the histograms have to be extracted.
        size_t frame; // Frame from which the histograms have to be extracted.
        std::pair<float, float> extremas; // Start and end points of all histograms.
        size_t nBins; // Number of bins (slots) in the histogram array
        float binSize; // Size of a bin (number of values covered by a slot)

        int run(const MaskCanvas*, Bucket b) override;
        int run(const LabeledCanvas*, Bucket b) override;

        int run(const VoxelsCanvasU8*, Bucket b) override;
        int run(const VoxelsCanvasU16*, Bucket b) override;
        int run(const VoxelsCanvasFloat*, Bucket b) override;

        int execute(Bucket b) override;

        HistogramSliceTask() = delete;
        HistogramSliceTask(Data* t, std::vector<Statistics>& s, Bucket d, size_t c, size_t f, std::pair<float, float> e, size_t nb, size_t bn);
    };

    std::vector<Bucket> make_buckets(size_t channel, size_t frame);
    void set_target(Data* t);

public:

    inline bool is_streamable() const override { return true; }

    void process_statistics(size_t channel=0, size_t frame=0);
    const std::vector<Statistics>& get_stats_by_slice();
    Statistics get_canvas_stats();

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
