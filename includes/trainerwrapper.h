#include "trainerwrapper_impl.h"

#ifdef NEURALNETLIB_EXPORTS
#define EXPORTING __declspec(dllexport)
#else
#define EXPORTING __declspec(dllimport)
#endif

#ifdef TrainerWrapper
#undef TrainerWrapper
#endif

#ifndef TrainerWrapper_API
#define TrainerWrapper_API TrainerWrapper
#endif


class TrainerWrapper_API {
public:
    TrainerWrapper_API() = delete;
    TrainerWrapper_API(int n_subjects, int network_size);
    ~TrainerWrapper_API();

    double** AddInputArray(int input_size);
    double** AddOutputArray(int output_size);
    void Step();
    void NextGeneration(int winner_id, int runner_up_id);
    bool CheckStatus();

private:
    TrainerWrapper_impl* pImpl;
};


// extern "C" EXPORTING TrainerWrapper * TrainerWrapper_Create(int n_subjects, int network_size);
// extern "C" EXPORTING double** TrainerWrapper_AddInputArray(TrainerWrapper * tw, int input_size);
// extern "C" EXPORTING double** TrainerWrapper_AddOutputArray(TrainerWrapper * tw, int output_size);
// extern "C" EXPORTING void TrainerWrapper_Step(TrainerWrapper * tw);
// extern "C" EXPORTING void TrainerWrapper_NextGeneration(TrainerWrapper * tw, int winner_id);
// extern "C" EXPORTING bool TrainerWrapper_CheckStatus(TrainerWrapper * tw);