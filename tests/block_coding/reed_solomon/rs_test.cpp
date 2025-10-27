#include <iostream>
#include <chrono>
#include "reed_solomon.h"

int main(int argc, char** argv)
{
	constexpr uint32_t code_length = reed_solomon::RsCoder_t::GetCodeLength();
	constexpr uint32_t data_length = reed_solomon::RsCoder_t::GetDataLength();
	reed_solomon::RsCoder_t* rs_coder = new reed_solomon::RsCoder_t(true,true);

	uint8_t message[3*code_length];
	memset(message,0x55,3*code_length);	
	uint32_t n = 3*data_length;
	uint8_t* block = rs_coder->Encode(message,n);

	printf("n=%d\n",n);

	block[0] = ~block[0];
	block[1] = ~block[1];
	block[2] = ~block[2];
	block[3] = ~block[3];
	block[4] = ~block[4];
	block[14] = ~block[14];
	block[15] = ~block[15];
	block[16] = ~block[16];
	block[17] = ~block[17];
	block[18] = ~block[18];

	uint8_t* decode = rs_coder->Decode(block, n);
	printf("n=%d\n",n);
	for(uint32_t j =0; j <n/data_length; j++ )
	{
		for(uint32_t i =0; i <data_length; i++ )
		{
			printf("0x%x ", decode[i]);
		}
		decode += data_length;
		printf("\n\n");
	}

	delete[] block;
	delete[] decode;
	return 0;
}

int main0(int argc, char** argv)
{
    std::chrono::time_point<std::chrono::steady_clock> t[5];

	  /* Reed Solomon Code Parameters */
	  const uint32_t code_length = reed_solomon::RsCoder_t::GetCodeLength();
	  const uint32_t data_length = reed_solomon::RsCoder_t::GetDataLength();

	  t[0] = std::chrono::steady_clock::now();
	  reed_solomon::RsCoder_t* rs_coder = new reed_solomon::RsCoder_t(true,true);

	  {
		  t[1] = std::chrono::steady_clock::now();
		  auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(t[1]-t[0]).count();
		  std::cout << "time new" << ": " <<  (double)dt/1e6 << "ms \n\n";
	  }

	   /* Pad message with nulls up until the code-word length */
	   uint8_t message[code_length];
	   memset(message,0x55,code_length);

	   /* Instantiate RS Block For Codec */	   
	   uint32_t repeat = 0;
	   do{

	   /* Transform message into Reed-Solomon encoded codeword */
	   t[0] = std::chrono::steady_clock::now();
	   uint32_t n0 = data_length;
	   uint8_t* block = rs_coder->Encode(message,n0);
	   t[1] = std::chrono::steady_clock::now();

	   /* Add errors at every 3rd location starting at position zero */
	   if(repeat == 0)
	   {

	   }
	   else if(repeat == 1)
	   {
		   block[0] = ~block[0] & 0xff;
		   block[1] = ~block[1] & 0xff;
		   block[2] = ~block[2] & 0xff;
		   block[3] = ~block[3] & 0xff;
	   }
	   else
	   {
		   block[0] = ~block[0] & 0xff;
		   block[1] = ~block[1] & 0xff;
		   block[2] = ~block[2] & 0xff;
		   block[3] = ~block[3] & 0xff;
		   block[4] = ~block[4] & 0xff;
	   }


	   t[2] = std::chrono::steady_clock::now();
	   uint32_t n = code_length;
	   uint8_t* decode = rs_coder->Decode(block, n);
	   t[3] = std::chrono::steady_clock::now();

       {
		auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(t[1]-t[0]).count();
		std::cout << "time encoding: "<<  (double)dt/1e6 << "ms \n";

		dt = std::chrono::duration_cast<std::chrono::nanoseconds>(t[3]-t[2]).count();
		std::cout << "time decoding: "<<  (double)dt/1e6 << "ms \n";
       }

	   for(uint32_t i =0; i <data_length; i++ )
	   {
		   printf("0x%x ", decode[i]);
	   }
	   printf("\n\n");

	   delete[] block;
	   delete[] decode;

	   }
	   while(repeat++ < 2);

	   return 0;
}

