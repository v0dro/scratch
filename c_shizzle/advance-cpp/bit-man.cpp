#include <bitset>
#include <iostream>

int main(int argc, char *argv[])
{
  int16_t num;
  num = 4;
  int16_t *num_address = &num;
  // unsigned long int temp = num_address;
  
  // std::bitset<64> num_address_bits(temp);

  size_t ptr_val = (size_t)num_address;
  size_t offset = (size_t)num_address & 2;

  std::cout << "offset: " << offset <<  " ptr_val: " << ptr_val << " num_address: " << num_address
            << " address as ui: " << (uint32_t*)((char*)num_address - offset) <<  std::endl;
  
  return 0;
}
