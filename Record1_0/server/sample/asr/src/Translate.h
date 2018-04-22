#ifndef TRANSLATE
#define TRANSLATE

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

//SDK interface
#include "BDSpeechSDK.hpp"
#include "bds_ASRDefines.hpp"
#include "bds_asr_key_definitions.hpp"

int Translate(const char *filename);
std::string get_gmt_time();
void asr_set_config_params(bds::BDSSDKMessage &cfg_params);
void asr_set_start_params(bds::BDSSDKMessage &start_params);
int asr_online_pushaudio(const char *file_path, const std::string &push_cmd, bds::BDSpeechSDK *sdk, int thread_seq);
void asr_output_callback(bds::BDSSDKMessage &message, void *user_arg);
void asr_online_release(bds::BDSpeechSDK *sdk);
int asr_online_stop(bds::BDSpeechSDK *sdk);

std::string GetResult();

extern char *result_buff;
extern int result_size;



#endif