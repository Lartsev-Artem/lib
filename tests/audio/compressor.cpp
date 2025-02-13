#include "compressor.h"

#include "file_module.h"
#include <string>


int main(int argc, char** argv)
{
    std::string file_input = "../../data/audio/voice_48000gz.bin";
	std::string file_output = "compessed_audio.bin";

	const uint32_t N = io_api::get_file_size(file_input.c_str())/sizeof(int16_t);
	int16_t* input_data = new int16_t[N];

	if(io_api::read_file(file_input.c_str(),N*sizeof(int16_t),input_data))
	{
		delete[] input_data;
		return 1;
	}

	compressor::settings_t params;
    compressor::init_low_compression(&params);

	Compressor compressor(&params, 48000);
	compressor.Update(input_data, input_data,N);

	io_api::write_file(file_output.c_str(), N*sizeof(int16_t),input_data);

	delete[] input_data;
	return 0;
}
