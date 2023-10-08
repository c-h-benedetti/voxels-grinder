#ifndef THRESHOLD_TASK_HPP_INCLUDED
#define THRESHOLD_TASK_HPP_INCLUDED

#include <memory>
#include <vector>
#include "Task.hpp"
#include "VoxelsCanvas.hpp"

class ThresholderTask : public Task {

public:

    enum class method {
        MANUAL    =0,
        OTSU      =1,
        YEN       =2,
        MEAN      =3,
        INTERMODES=4,
        KAPUR     =5,
        SHANBHAG  =6,
        LI        =7,
        ROSIN     =8,
        ADAPTIVE  =9,
        SAUVOLA   =10,
        PHANSALKAR=11,
        TRIANGLE  =12,
        MEDIAN    =13,
        HYSTERESIS=14
    };

private:

    /**
     * Image that must be thresholded. 
     * Tasks don't modify input images, hence the 'const' flag. 
     * Input's life is not the task's responsability, so we have a raw pointer.
     */
    const VoxelsCanvas* target;

    /**
     * Output's life is our responsability until it is captured by somebody.
     * At the moment it is accessed, the task doesn't own it anymore.
     */
    std::unique_ptr<VoxelsCanvas> output;
    
    /**
     * Lower and upper boundaries of the threshold.
     * Everything below the lower and above the upper is considered as being part of the background.
     * Even indices are lower bounds while odd indices are upper bounds.
     * There is a pair of value for each slice on the target.
     */
    std::vector<float> bounds;
    
    /**
     * Thresholding method that we want to use.
     * Manual thresholding is the default and requires the user to provide values himself.
     */
    method t_method;

    /**
     * If true, we will apply a threshold slice by slice rather than on the whole image.
     * If we are in manual mode, this value doesn't have any effet.
     */
    bool split_slices=false;

private:

    bool estimate_otsu(Bucket b);
    bool estimate_yen(Bucket b);
    bool estimate_mean(Bucket b);
    bool estimate_intermodes(Bucket b);
    bool estimate_kapur(Bucket b);
    bool estimate_shanbhag(Bucket b);
    bool estimate_li(Bucket b);
    bool estimate_rosin(Bucket b);
    bool estimate_adaptive(Bucket b);
    bool estimate_sauvola(Bucket b);
    bool estimate_phansalkar(Bucket b);
    bool estimate_triangle(Bucket b);
    bool estimate_median(Bucket b);
    bool estimate_hysteresis(Bucket b);

    bool set_target(Data* d);

public:

    ThresholderTask(Data* d, method m=method::MANUAL, bool by_slice=false);

    /**
    * Estimate the lower and upper bounds when we are in auto-thresholding mode.
    * Doesn't do anything in manual mode.
    */
    void process_bounds();

    float get_lower_bound(size_t slice=0);

    float get_upper_bound(size_t slice=0);

    /**
    * Determine which auto-thresholding method is going to be used for the current target.
    * If set to NONE, the manual thresholding is re-activated.
    */
    inline void set_threshold_method(method use) { t_method = use; }

    /**
    * Manually set the upper and lower bounds used to threshold the image.
    * Can also be used to reset both bounds if no argument is provided.
    */
    void set_bounds(float lower=std::numeric_limits<float>::min(), float upper=std::numeric_limits<float>::max(), size_t slice);

    /**
     * Uses the formerly processed bounds to create a mask from the target image.
     */
    void apply_threshold();

    /**
     * Transfers the output's ownership.
     */
    inline std::unique_ptr<VoxelsCanvas> get_mask() { return std::move(this->output); }

};

#endif //THRESHOLD_TASK_HPP_INCLUDED

/** NOTE:
*
* > Pourquoi doit-on construire un objet à chaque fois qu'on veut lancer l'opération ? 
*   Pourquoi ne peut-on juste pas modifier la cible et passer à l'image suivante ?
*
* On veut pouvoir empêcher les opérations de se réaliser si l'input n'est pas valide.
* Si on peut modifier la cible, on doit vérifier dans le corps de chaque fonction que la cible existe.
* En effet, si l'utilisateur peut changer la cible, elle peut être à nullptr à n'importe quel moment.
* L'implémentation a donc été faite de sorte à ce que le constructeur prenne la cible.
* Ainsi, tout va bien si la cible est valide et on peut lever une exception dès le constructeur si la cible est invalide.
* 
* > On pourrait simplement avoir une fonction qui réalise cette tâche sans avoir un objet !
* 
* Dans le cas du seuillage, nous pourrions en effet utiliser de simple fonctions (avec beaucoup de surcharges).
* En revanche, dans le cas d'opérations qui produisent beaucoup de résultats différents (comme regionprops),
* on veut pouvoir stocker tous ces résultats jusqu'à ce qu'ils soient récupérés (une fonction retourne au plus un élément).
* De plus, certaines opérations auront besoin d'une "live preview" et cela sera beaucoup plus simple à implémenter avec un objet.
* Le canvas de preview sera stocké dans l'instance et capturé en direct. Pas besoin de retourner quoi que ce soit.
*
* > J'aimerais choisir les couleurs de background et foreground pour mon masque, mais aucune méthode ne change cela.
*
* La gestion des couleurs de foreground et background est faite globalement (à l'échelle de l'environnement) plutôt que dans les tasks.
* Les valeur de FG et BG sont à changer à globalement.
* Notez néanmoins que certaines fonctions qui ne sont pas maintenues par les développeurs de VoxelsGrinder ne permettent pas de modifier ces valeurs.
* Elles considèrent que le BG est 0 et le FG 255 (ou autre selon le type de données).
* Si vous voulez un background blanc mais toujours compatible avec ces fonctions, il est donc préférable d'utiliser une LUT.

* > Je ne sais pas vraiment quel algorithme de seuillage choisir, comment décider.
*
* Dans de rares situations, un algorithme de thresholding a été déterminé comme étant celui qui fonctionne le mieux.
* Cependant, dans la majorité des cas, le "bon" algorithme est déterminé par essai-erreur.
* On prend simplement celui qui donne la meilleure séparation entre FG et BG.
* On peut faire un choix soit avec la live preview, soit en utilisant l'action qui permet de faire un montage montrant le résultat de chaque algo.
* 
* > J'ai un stack mais je veux faire un threshold slice par slice plutôt que sur la globalité de l'image.
*   De la même façon, j'ai plusieurs channels et je veux ne faire un seuillage que sur l'un d'entre-eux.
* 
* Par défaut, VG utilise l'histogramme de l'image entière (donc toutes les slices) pour calculer son seuillage.
* Les histogrammes sont quand même séparés par channel et par frame.
* 
*/

/** TODO:

- [ ] A function that applies every possible method to a given image.
- [ ] Some canvas to allow the user to choose the threshold interactively.

*/