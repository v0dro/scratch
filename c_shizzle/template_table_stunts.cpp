namespace nm {

}

#define NAMED_DTYPE_TEMPLATE_TABLE(name, fun, ret, ...) \
  static ret (*(name)[nm::NUM_DTYPES])(__VA_ARGS__) = { \
    fun<uint8_t>,                                       \
    fun<int8_t>,                                        \
    fun<int16_t>,                                       \
    fun<int32_t>,                                       \
    fun<int64_t>,                                       \
    fun<float32_t>,                                     \
    fun<float64_t>,                                     \
    fun<nm::Complex64>,                                 \
    fun<nm::Complex128>,                                \
    fun<nm::Rational32>,                                \
    fun<nm::Rational64>,                                \
    fun<nm::Rational128>,                               \
    fun<nm::RubyObject>                                 \
  };

NAMED_DTYPE_TEMPLATE_TABLE(ttable, math, int, float, double, VALUE)

// Produces
// static int (*(ttable)[nm::NUM_DTYPES])(float, double, VALUE) = { 
//   math<uint8_t>, 
//   math<int8_t>, 
//   math<int16_t>, 
//   math<int32_t>, 
//   math<int64_t>, 
//   math<float32_t>, 
//   math<float64_t>, 
//   math<nm::Complex64>, 
//   math<nm::Complex128>, 
//   math<nm::Rational32>, 
//   math<nm::Rational64>, 
//   math<nm::Rational128>, 
//   math<nm::RubyObject> };