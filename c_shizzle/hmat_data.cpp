std::vector<double> get_data(std::vector<double> arr, int lower, int upper, int n)
{
  for (int i = 0; i < dim[0]; ++i) {
    for (int j = 0; j < dim[1]; ++j) {
      if (((*this).get_block(i,j)).is(HICMA_HIERARCHICAL)) {
        int u_b = dim[1] - (i * dim[1] + j);
        if (u_b == 0) {
          u_b = 0;
        }
        else {
          u_b = upper / u_b;
        }

        int l_b = i * dim[1] + j;
        if (l_b == 0) {
          l_b = lower;
        }
        else {
          l_b = lower + lower/l_b;
        }
        
        ((*this).get_block(i,j)).get_data(arr, l_b, u_b, n);
      }
      else if (((*this).get_block(i,j)).is(HICMA_DENSE)) {
        int j = 0;
        for (int i = lower; i < upper; ++i) {
          arr[i] = data[j++];
        }
      }
    }
  }
}
