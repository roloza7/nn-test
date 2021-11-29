#include "trainerwrapper.h"

#ifdef TrainerWrapper
#undef TrainerWrapper
#endif

#ifndef TrainerWrapper_API
#define TrainerWrapper_API TrainerWrapper
#endif

TrainerWrapper_API::TrainerWrapper_API(int n_subjects, int network_size) {
    pImpl = new TrainerWrapper_impl(n_subjects, network_size);
}

TrainerWrapper_API::~TrainerWrapper_API() {
    delete pImpl;
}

// double** TrainerWrapper_API::AddInputArray(int input_size) {
//     return pImpl->AddInputArray(input_size);
// }

// double** TrainerWrapper_API::AddOutputArray(int output_size) {
//     return pImpl->AddOutputArray(output_size);
// }

// bool TrainerWrapper_API::CheckStatus() {
//     return true;
// }

// void TrainerWrapper_API::Step() {
//     pImpl->Step();
// }

// void TrainerWrapper_API::NextGeneration(int winner_id) {
//     pImpl->NextGeneration(winner_id);
// }

// TrainerWrapper* TrainerWrapper_Create(int n_subjects, int network_size) {
//     return new TrainerWrapper(n_subjects, network_size);
// }

// double** TrainerWrapper_AddInputArray(TrainerWrapper* tw, int input_size) {
//     return tw->AddInputArray(input_size);
// }

// double** TrainerWrapper_AddOutputArray(TrainerWrapper* tw, int output_size) {
//     return tw->AddOutputArray(output_size);
// }

// void TrainerWrapper_Step(TrainerWrapper* tw) {
//     tw->Step();
// }

// void TrainerWrapper_NextGeneration(TrainerWrapper* tw, int winner_id) {
//     tw->NextGeneration(winner_id);
// }

// bool TrainerWrapper_CheckStatus(TrainerWrapper* tw) {
//     return tw->CheckStatus();
// }