// This file is generated. Do not edit.
// Generated on: 13.11.2025 15:27:36


#include "lib_tflite_micro/api/xcore_config.h"
#include "lib_nn/api/version.h"
#include "lib_tflite_micro/api/version.h"
#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/kernels/conv.h"
#include "tensorflow/lite/micro/kernels/fully_connected.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/kernels/reduce.h"
#include "tensorflow/lite/micro/kernels/softmax.h"
#include "tensorflow/lite/micro/micro_context.h"

// #define TFLMC_XCORE_PROFILE
// #define TFLMC_CONV2D_PROFILE
// #define TFLMC_PRINT_TENSORS
// #define TFLMC_PRINT_INPUT_TENSORS

#if defined __GNUC__
#define ALIGN(X) __attribute__((aligned(X)))
#elif defined _MSC_VER
#define ALIGN(X) __declspec(align(X))
#elif defined __TASKING__
#define ALIGN(X) __align(X)
#endif

// Check lib_nn and lib_tflite_micro versions
// NOTE: xformer version is saved for debugging purposes
// If lib_nn and lib_tflite_micro versions are as expected,
// then the xformer version doesn't matter as the model should execute
// If major version is zero, then minor versions must match
// Otherwise, major versions must match and binary minor version
// must be less or equal to runtime minor version
// Check if runtime lib_tflite_micro version matches with compiled version
static_assert((0 == 0 && lib_tflite_micro::major_version == 0 && 6 == lib_tflite_micro::minor_version) ||
              (0 == lib_tflite_micro::major_version) ||
              (6  < lib_tflite_micro::minor_version),
             "Model has been compiled with lib_tflite_micro version incompatible with runtime lib_tflite_micro version!");

// Check if runtime lib_nn version matches with compiled version
static_assert((0 == 0 && lib_nn::major_version == 0 && 3 == lib_nn::minor_version) ||
              (0 == lib_nn::major_version) ||
              (3  < lib_nn::minor_version),
             "Model has been compiled with lib_nn version incompatible with runtime lib_nn version!");

namespace tflite {
namespace ops {
namespace micro {
namespace xcore {
extern TFLMRegistration *Register_XC_ld_weights(void);
extern TFLMRegistration *Register_XC_conv2d_v2(void);
extern TFLMRegistration *Register_XC_slice(void);
extern TFLMRegistration *Register_XC_lookup(void);
} // namespace xcore
}  // namespace micro
}  // namespace ops
}  // namespace tflite



constexpr int kTensorArenaSize = 5136;
#ifndef SHARED_TENSOR_ARENA
namespace {
uint8_t tensor_arena[kTensorArenaSize] ALIGN(8);
}
#else
extern uint8_t tensor_arena[];
#endif

namespace {
template <int SZ, class T> struct TfArray {
  int sz; T elem[SZ];
};
enum used_operators_e {
  OP_XC_ld_weights, OP_XC_conv2d_v2, OP_XC_slice, OP_RESHAPE, OP_XC_lookup,  OP_LAST
};

#if defined(TFLMC_XCORE_PROFILE) || defined(TFLMC_PRINT_TENSORS) || defined(TFLMC_PRINT_INPUT_TENSORS) || defined(TFLMC_CONV2D_PROFILE)
const char *op_strs[] = {
"OP_XC_ld_weights", "OP_XC_conv2d_v2", "OP_XC_slice", "OP_RESHAPE", "OP_XC_lookup", };

#endif
#if defined(TFLMC_XCORE_PROFILE) || defined(TFLMC_PRINT_TENSORS) || defined(TFLMC_PRINT_INPUT_TENSORS)
unsigned char checksum(char *data, unsigned int length)
{
  static char sum;
  static char * end;
  sum = 0;
  end = data + length;

  do
  {
      sum -= *data++;
  } while (data != end);
  return sum;
}

#endif

#ifdef TFLMC_XCORE_PROFILE
int op_times[OP_LAST];
int op_counts[OP_LAST];
int64_t op_times_summed;
int time_t0, time_t1;
#endif

TfLiteContext ctx{};

TFLMRegistration registrations[OP_LAST];

struct {
const TfArray<4, int> tensor_dimension0 = { 4, { 1,1,4,24 } };
const TfArray<1, float> quant0_scale = { 1, { 0.11438909918069839, } };
const TfArray<1, int> quant0_zero = { 1, { 127 } };
const TfLiteAffineQuantization quant0 = { (TfLiteFloatArray*)&quant0_scale, (TfLiteIntArray*)&quant0_zero, 0 };
const ALIGN(8) int32_t tensor_data1[2] = { 
    1, 1, 
};
const TfArray<1, int> tensor_dimension1 = { 1, { 2 } };
const ALIGN(8) int16_t tensor_data2[20] = { 
    22083, 0, 0, 0, -5958, 3967, 3967, 3967, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};
const TfArray<1, int> tensor_dimension2 = { 1, { 20 } };
const TfArray<1, int> tensor_dimension3 = { 1, { 64 } };
const TfArray<1, int> tensor_dimension4 = { 1, { 1664 } };
const TfArray<1, int> tensor_dimension5 = { 1, { 128 } };
const TfArray<4, int> tensor_dimension6 = { 4, { 1,1,4,64 } };
const TfArray<1, float> quant6_scale = { 1, { 0.24111956357955933, } };
const TfArray<1, int> quant6_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant6 = { (TfLiteFloatArray*)&quant6_scale, (TfLiteIntArray*)&quant6_zero, 0 };
const TfArray<1, int> tensor_dimension7 = { 1, { 2048 } };
const TfArray<1, int> tensor_dimension8 = { 1, { 96 } };
const TfArray<4, int> tensor_dimension9 = { 4, { 1,1,4,32 } };
const TfArray<1, float> quant9_scale = { 1, { 0.33315452933311462, } };
const TfArray<1, int> quant9_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant9 = { (TfLiteFloatArray*)&quant9_scale, (TfLiteIntArray*)&quant9_zero, 0 };
const TfArray<1, int> tensor_dimension10 = { 1, { 4096 } };
const TfArray<4, int> tensor_dimension12 = { 4, { 1,1,1,32 } };
const TfArray<1, float> quant12_scale = { 1, { 0.79527455568313599, } };
const TfArray<1, int> quant12_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant12 = { (TfLiteFloatArray*)&quant12_scale, (TfLiteIntArray*)&quant12_zero, 0 };
const TfArray<1, int> tensor_dimension14 = { 1, { 512 } };
const TfArray<4, int> tensor_dimension15 = { 4, { 1,1,1,4 } };
const TfArray<1, float> quant15_scale = { 1, { 0.17188803851604462, } };
const TfArray<1, int> quant15_zero = { 1, { 31 } };
const TfLiteAffineQuantization quant15 = { (TfLiteFloatArray*)&quant15_scale, (TfLiteIntArray*)&quant15_zero, 0 };
const TfArray<4, int> tensor_dimension16 = { 4, { 1,1,1,1 } };
const TfArray<2, int> tensor_dimension17 = { 2, { 1,1 } };
const TfArray<1, int> tensor_dimension18 = { 1, { 256 } };
const TfArray<1, float> quant19_scale = { 1, { 0.00390625, } };
const TfArray<1, int> quant19_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant19 = { (TfLiteFloatArray*)&quant19_scale, (TfLiteIntArray*)&quant19_zero, 0 };
uint8_t ALIGN(4) opdata0[46] = { 97, 100, 100, 114, 0, 115, 105, 122, 101, 115, 0, 0, 2, 0, 128, 6, 0, 1, 5, 5, 100, 100, 114, 0, 3, 25, 6, 22, 3, 0, 1, 0, 3, 0, 64, 27, 0, 0, 24, 0, 5, 105, 41, 9, 37, 1,  }; /* custom_initial_data */
const int inputs0 = 0; /* empty TfLiteIntArray */
const TfArray<2, int> outputs0 = { 2, { 4,5 } };
uint8_t ALIGN(4) opdata1[148] = { 109, 112, 0, 40, 96, 0, 0, 0, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 232, 255, 255, 255, 248, 255, 255, 255, 72, 0, 0, 0, 255, 255, 255, 0, 0, 0, 0, 0, 0, 97, 0, 8, 64, 0, 0, 0, 24, 0, 0, 0, 0, 111, 0, 8, 64, 0, 0, 0, 3, 0, 251, 255, 0, 112, 0, 38, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 48, 48, 0, 1, 40, 20, 115, 0, 107, 0, 116, 0, 7, 76, 6, 123, 67, 56, 12, 9, 7, 1, 7, 83, 1, 129, 74, 23, 64, 0, 20, 4, 20, 20, 40, 4, 4, 14, 36, 1,  }; /* custom_initial_data */
const TfArray<5, int> inputs1 = { 5, { 0,4,5,-1,3 } };
const TfArray<1, int> outputs1 = { 1, { 6 } };
uint8_t ALIGN(4) opdata2[46] = { 97, 100, 100, 114, 0, 115, 105, 122, 101, 115, 0, 0, 2, 0, 0, 8, 192, 0, 5, 5, 100, 100, 114, 0, 3, 25, 6, 22, 3, 0, 1, 0, 3, 0, 128, 18, 0, 0, 24, 0, 5, 105, 41, 9, 37, 1,  }; /* custom_initial_data */
const int inputs2 = 0; /* empty TfLiteIntArray */
const TfArray<2, int> outputs2 = { 2, { 7,8 } };
uint8_t ALIGN(4) opdata3[132] = { 109, 112, 0, 8, 0, 1, 0, 0, 64, 0, 0, 0, 0, 97, 0, 24, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 192, 0, 0, 0, 0, 111, 0, 8, 32, 0, 0, 0, 249, 255, 0, 0, 0, 112, 0, 38, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 48, 48, 0, 1, 40, 20, 115, 0, 107, 0, 116, 0, 7, 92, 6, 107, 67, 56, 12, 9, 7, 1, 7, 99, 0, 113, 74, 23, 0, 1, 20, 4, 20, 20, 40, 4, 4, 14, 36, 1,  }; /* custom_initial_data */
const TfArray<5, int> inputs3 = { 5, { 6,7,8,-1,-1 } };
const TfArray<1, int> outputs3 = { 1, { 9 } };
uint8_t ALIGN(4) opdata4[46] = { 97, 100, 100, 114, 0, 115, 105, 122, 101, 115, 0, 0, 2, 0, 0, 16, 128, 0, 5, 5, 100, 100, 114, 0, 3, 25, 6, 22, 3, 0, 1, 0, 3, 0, 0, 2, 0, 0, 24, 0, 5, 105, 41, 9, 37, 1,  }; /* custom_initial_data */
const int inputs4 = 0; /* empty TfLiteIntArray */
const TfArray<2, int> outputs4 = { 2, { 10,11 } };
uint8_t ALIGN(4) opdata5[132] = { 109, 112, 0, 8, 128, 0, 0, 0, 128, 0, 0, 0, 0, 97, 0, 24, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 111, 0, 8, 32, 0, 0, 0, 1, 0, 252, 255, 0, 112, 0, 38, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 48, 48, 0, 1, 40, 20, 115, 0, 107, 0, 116, 0, 7, 92, 6, 107, 67, 56, 12, 9, 7, 1, 7, 99, 0, 113, 74, 23, 0, 0, 20, 4, 20, 20, 40, 4, 4, 14, 36, 1,  }; /* custom_initial_data */
const TfArray<5, int> inputs5 = { 5, { 9,10,11,-1,-1 } };
const TfArray<1, int> outputs5 = { 1, { 12 } };
uint8_t ALIGN(4) opdata6[37] = { 97, 100, 100, 114, 0, 115, 105, 122, 101, 115, 0, 0, 1, 0, 0, 2, 5, 100, 100, 114, 0, 3, 22, 6, 19, 3, 1, 3, 0, 0, 16, 4, 104, 41, 6, 36, 1,  }; /* custom_initial_data */
const int inputs6 = 0; /* empty TfLiteIntArray */
const TfArray<1, int> outputs6 = { 1, { 14 } };
uint8_t ALIGN(4) opdata7[148] = { 109, 112, 0, 40, 32, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 224, 255, 255, 255, 0, 0, 0, 0, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 0, 97, 0, 8, 4, 0, 0, 0, 32, 0, 0, 0, 0, 111, 0, 8, 4, 0, 0, 0, 3, 0, 255, 255, 0, 112, 0, 38, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 48, 48, 0, 1, 40, 20, 115, 0, 107, 0, 116, 0, 7, 76, 6, 123, 67, 56, 12, 9, 7, 1, 7, 83, 1, 129, 74, 23, 64, 0, 20, 4, 20, 20, 40, 4, 4, 14, 36, 1,  }; /* custom_initial_data */
const TfArray<5, int> inputs7 = { 5, { 12,14,2,-1,13 } };
const TfArray<1, int> outputs7 = { 1, { 15 } };
uint8_t ALIGN(4) opdata8[32] = { 115, 0, 111, 0, 108, 0, 110, 0, 118, 0, 5, 7, 6, 11, 14, 7, 5, 1, 5, 1, 1, 4, 0, 0, 4, 4, 4, 4, 104, 10, 36, 1,  }; /* custom_initial_data */
const TfArray<1, int> inputs8 = { 1, { 15 } };
const TfArray<1, int> outputs8 = { 1, { 16 } };
const TfLiteReshapeParams opdata9 = { { 0, 0, 0, 0, 0, 0, 0, 0, }, 0 };
const TfArray<2, int> inputs9 = { 2, { 16,1 } };
const TfArray<1, int> outputs9 = { 1, { 17 } };
uint8_t ALIGN(4) opdata10[44] = { 97, 100, 100, 114, 0, 115, 105, 122, 101, 115, 0, 0, 1, 0, 0, 1, 5, 100, 100, 114, 0, 3, 22, 6, 19, 0, 4, 0, 1, 0, 3, 0, 192, 34, 0, 0, 22, 0, 5, 105, 41, 9, 37, 1,  }; /* custom_initial_data */
const int inputs10 = 0; /* empty TfLiteIntArray */
const TfArray<1, int> outputs10 = { 1, { 18 } };
uint8_t ALIGN(4) opdata11[0] = {  }; /* custom_initial_data */
const TfArray<2, int> inputs11 = { 2, { 17,18 } };
const TfArray<1, int> outputs11 = { 1, { 19 } };
} g0;

TfLiteTensor tflTensors[] = 
{{ {(int32_t*)(tensor_arena + 1920)},(TfLiteIntArray*)&g0.tensor_dimension0, kTfLiteInt8, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&g0.quant0)) }, {g0.quant0.scale->data[0], g0.quant0.zero_point->data[0] },96, kTfLiteArenaRw, false, },
{ {(int32_t*)g0.tensor_data1},(TfLiteIntArray*)&g0.tensor_dimension1, kTfLiteInt32, {kTfLiteNoQuantization, nullptr }, {0,0},8, kTfLiteMmapRo, false, },
{ {(int32_t*)g0.tensor_data2},(TfLiteIntArray*)&g0.tensor_dimension2, kTfLiteInt16, {kTfLiteNoQuantization, nullptr }, {0,0},40, kTfLiteMmapRo, false, },
{ {(int32_t*)(tensor_arena + 2304)},(TfLiteIntArray*)&g0.tensor_dimension3, kTfLiteInt8, {kTfLiteNoQuantization, nullptr }, {0,0},64, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 0)},(TfLiteIntArray*)&g0.tensor_dimension4, kTfLiteInt8, {kTfLiteNoQuantization, nullptr }, {0,0},1664, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 1664)},(TfLiteIntArray*)&g0.tensor_dimension5, kTfLiteInt16, {kTfLiteNoQuantization, nullptr }, {0,0},256, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 2048)},(TfLiteIntArray*)&g0.tensor_dimension6, kTfLiteInt8, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&g0.quant6)) }, {g0.quant6.scale->data[0], g0.quant6.zero_point->data[0] },256, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 0)},(TfLiteIntArray*)&g0.tensor_dimension7, kTfLiteInt8, {kTfLiteNoQuantization, nullptr }, {0,0},2048, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 2304)},(TfLiteIntArray*)&g0.tensor_dimension8, kTfLiteInt16, {kTfLiteNoQuantization, nullptr }, {0,0},192, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 4224)},(TfLiteIntArray*)&g0.tensor_dimension9, kTfLiteInt8, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&g0.quant9)) }, {g0.quant9.scale->data[0], g0.quant9.zero_point->data[0] },128, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 0)},(TfLiteIntArray*)&g0.tensor_dimension10, kTfLiteInt8, {kTfLiteNoQuantization, nullptr }, {0,0},4096, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 4096)},(TfLiteIntArray*)&g0.tensor_dimension3, kTfLiteInt16, {kTfLiteNoQuantization, nullptr }, {0,0},128, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 4352)},(TfLiteIntArray*)&g0.tensor_dimension12, kTfLiteInt8, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&g0.quant12)) }, {g0.quant12.scale->data[0], g0.quant12.zero_point->data[0] },32, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 512)},(TfLiteIntArray*)&g0.tensor_dimension3, kTfLiteInt8, {kTfLiteNoQuantization, nullptr }, {0,0},64, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 0)},(TfLiteIntArray*)&g0.tensor_dimension14, kTfLiteInt8, {kTfLiteNoQuantization, nullptr }, {0,0},512, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 576)},(TfLiteIntArray*)&g0.tensor_dimension15, kTfLiteInt8, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&g0.quant15)) }, {g0.quant15.scale->data[0], g0.quant15.zero_point->data[0] },4, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 0)},(TfLiteIntArray*)&g0.tensor_dimension16, kTfLiteInt8, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&g0.quant15)) }, {g0.quant15.scale->data[0], g0.quant15.zero_point->data[0] },1, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 256)},(TfLiteIntArray*)&g0.tensor_dimension17, kTfLiteInt8, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&g0.quant15)) }, {g0.quant15.scale->data[0], g0.quant15.zero_point->data[0] },1, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 0)},(TfLiteIntArray*)&g0.tensor_dimension18, kTfLiteUInt8, {kTfLiteNoQuantization, nullptr }, {0,0},256, kTfLiteArenaRw, false, },
{ {(int32_t*)(tensor_arena + 256)},(TfLiteIntArray*)&g0.tensor_dimension17, kTfLiteInt8, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&g0.quant19)) }, {g0.quant19.scale->data[0], g0.quant19.zero_point->data[0] },1, kTfLiteArenaRw, false, },
};

TfLiteNode tflNodes[] = 
{{ (TfLiteIntArray*)&g0.inputs0, (TfLiteIntArray*)&g0.outputs0, (TfLiteIntArray*)&g0.inputs0, const_cast<void*>(static_cast<const void*>(&g0.opdata0)), 46, },
{ (TfLiteIntArray*)&g0.inputs1, (TfLiteIntArray*)&g0.outputs1, (TfLiteIntArray*)&g0.inputs1, const_cast<void*>(static_cast<const void*>(&g0.opdata1)), 148, },
{ (TfLiteIntArray*)&g0.inputs2, (TfLiteIntArray*)&g0.outputs2, (TfLiteIntArray*)&g0.inputs2, const_cast<void*>(static_cast<const void*>(&g0.opdata2)), 46, },
{ (TfLiteIntArray*)&g0.inputs3, (TfLiteIntArray*)&g0.outputs3, (TfLiteIntArray*)&g0.inputs3, const_cast<void*>(static_cast<const void*>(&g0.opdata3)), 132, },
{ (TfLiteIntArray*)&g0.inputs4, (TfLiteIntArray*)&g0.outputs4, (TfLiteIntArray*)&g0.inputs4, const_cast<void*>(static_cast<const void*>(&g0.opdata4)), 46, },
{ (TfLiteIntArray*)&g0.inputs5, (TfLiteIntArray*)&g0.outputs5, (TfLiteIntArray*)&g0.inputs5, const_cast<void*>(static_cast<const void*>(&g0.opdata5)), 132, },
{ (TfLiteIntArray*)&g0.inputs6, (TfLiteIntArray*)&g0.outputs6, (TfLiteIntArray*)&g0.inputs6, const_cast<void*>(static_cast<const void*>(&g0.opdata6)), 37, },
{ (TfLiteIntArray*)&g0.inputs7, (TfLiteIntArray*)&g0.outputs7, (TfLiteIntArray*)&g0.inputs7, const_cast<void*>(static_cast<const void*>(&g0.opdata7)), 148, },
{ (TfLiteIntArray*)&g0.inputs8, (TfLiteIntArray*)&g0.outputs8, (TfLiteIntArray*)&g0.inputs8, const_cast<void*>(static_cast<const void*>(&g0.opdata8)), 32, },
{ (TfLiteIntArray*)&g0.inputs9, (TfLiteIntArray*)&g0.outputs9, (TfLiteIntArray*)&g0.inputs9, const_cast<void*>(static_cast<const void*>(&g0.opdata9)), 0, },
{ (TfLiteIntArray*)&g0.inputs10, (TfLiteIntArray*)&g0.outputs10, (TfLiteIntArray*)&g0.inputs10, const_cast<void*>(static_cast<const void*>(&g0.opdata10)), 44, },
{ (TfLiteIntArray*)&g0.inputs11, (TfLiteIntArray*)&g0.outputs11, (TfLiteIntArray*)&g0.inputs11, const_cast<void*>(static_cast<const void*>(&g0.opdata11)), 0, },
};

used_operators_e used_ops[] =
{OP_XC_ld_weights, OP_XC_conv2d_v2, OP_XC_ld_weights, OP_XC_conv2d_v2, OP_XC_ld_weights, OP_XC_conv2d_v2, OP_XC_ld_weights, OP_XC_conv2d_v2, OP_XC_slice, OP_RESHAPE, OP_XC_ld_weights, OP_XC_lookup, };


// Indices into tflTensors and tflNodes for subgraphs
size_t tflTensors_subgraph_index[] = {0, 20, };
size_t tflNodes_subgraph_index[] = {0, 12, };

// Variable tensors
size_t varTensors_index[] = {};

// Input/output tensors
static const int inTensorIndices[] = {
  0, 
};

static const int outTensorIndices[] = {
  19, 
};

// Indices into inTensors and outTensors for subgraphs
size_t inTensors_subgraph_index[] = {0, 1, };
size_t outTensors_subgraph_index[] = {0, 1, };

// Scratch buffer variables
int scratch_buffer_idx;
const int scratch_buffer_offsets[0] = {  };
tflite::MicroContext mc;
tflite::MicroGraph micro_graph;
size_t currentSubgraphIndex = 0;

// Xcore context and thread variables
xc_context_config_t xc_config;
// When using USE_DDR_FIX for enabling LPDDR support, only one thread can be used
#ifdef USE_DDR_FIX
static_assert((1 == 1),
             "Only one thread can be used when using USE_DDR_FIX! Please recompile with one thread!");
#endif
constexpr int kStackWordsPerThread = 256;
constexpr int threadsStackSizeInUint64 = 1 * kStackWordsPerThread/2;
// We use uint64_t for xcThreadsStack so that it is aligned to 8 bytes
uint64_t xcThreadsStack[threadsStackSizeInUint64];

// Persistent buffer ptr
// Initialized to the tail end of the tensor arena
uint8_t *persistentBufferPtr;
// Functions to be used as function pointers for TfLiteContext and MicroContext 
static void* AllocatePersistentBuffer(struct TfLiteContext* ctx,
                                                 size_t bytes) {
  // Align to double word
  bytes = ((bytes + 7) / 8) * 8;
  persistentBufferPtr -= bytes;
  return persistentBufferPtr;
}

static TfLiteEvalTensor *GetEvalTensor(const struct TfLiteContext *context,
                                       int tensor_idx) {
  return (TfLiteEvalTensor*)&tflTensors[tflTensors_subgraph_index[currentSubgraphIndex] + tensor_idx];
}

static TfLiteStatus RequestScratchBufferInArena(struct TfLiteContext *context, size_t bytes,
                                       int *buffer_idx) {
  *buffer_idx = scratch_buffer_idx++;
  return kTfLiteOk;
};

static void *GetScratchBuffer(struct TfLiteContext *context,
                                       int buffer_idx) {
  return tensor_arena + scratch_buffer_offsets[buffer_idx];
}

static TfLiteTensor* mc_AllocateTempInputTensor(const TfLiteNode* node, int index) {
      if (node->inputs->data[index] < 0) {
        return nullptr;
      }
      return &ctx.tensors[tflTensors_subgraph_index[currentSubgraphIndex] + node->inputs->data[index]];
}

static TfLiteTensor* mc_AllocateTempOutputTensor(const TfLiteNode* node, int index) {
      if (node->outputs->data[index] < 0) {
        return nullptr;
      }
      return &ctx.tensors[tflTensors_subgraph_index[currentSubgraphIndex] + node->outputs->data[index]];
}

static void mc_DeallocateTempTfLiteTensor(TfLiteTensor* tensor) {
}

static void* mc_external_context() {
  return &xc_config;
}

static tflite::MicroGraph& mc_graph() {
  return micro_graph;
}

static int mg_NumSubgraphs(){
  return sizeof(tflTensors_subgraph_index)/sizeof(size_t) - 1;
}

static size_t mg_NumSubgraphInputs(int subgraph_idx){
  return inTensors_subgraph_index[subgraph_idx+1] - inTensors_subgraph_index[subgraph_idx];
}

static size_t mg_NumSubgraphOutputs(int subgraph_idx){
  return outTensors_subgraph_index[subgraph_idx+1] - outTensors_subgraph_index[subgraph_idx];
}

static TfLiteEvalTensor* mg_GetSubgraphInput(int subgraph_idx, int i){
  return (TfLiteEvalTensor*)&tflTensors[tflTensors_subgraph_index[subgraph_idx] + inTensorIndices[inTensors_subgraph_index[subgraph_idx] + i]];
}

static TfLiteEvalTensor* mg_GetSubgraphOutput(int subgraph_idx, int i){
  return (TfLiteEvalTensor*)&tflTensors[tflTensors_subgraph_index[subgraph_idx] + outTensorIndices[outTensors_subgraph_index[subgraph_idx] + i]];
}

static TfLiteStatus mg_InvokeSubgraph(int g){
  int prevSubgraphIndex = currentSubgraphIndex;
  currentSubgraphIndex = g;
#ifdef TFLMC_PRINT_TENSORS
printf("[\n");
#endif

  for(size_t i = tflNodes_subgraph_index[g]; i < tflNodes_subgraph_index[g+1]; ++i) {

#ifdef TFLMC_PRINT_INPUT_TENSORS
    // print every input tensor
    printf("\nnode in %d", i);
    for (int j=0; j<tflNodes[i].inputs->size; j++){
      // -1 such as in case of no bias tensor for conv
      if (tflNodes[i].inputs->data[j] != -1) {
        printf("\ntensor %d, input %d, %d bytes, checksum %d\n", tflNodes[i].inputs->data[j], j, tflTensors[tflNodes[i].inputs->data[j]].bytes, checksum(tflTensors[tflNodes[i].inputs->data[j]].data.raw, tflTensors[tflNodes[i].inputs->data[j]].bytes));
        for(int k=0; k<tflTensors[tflTensors_subgraph_index[g] + tflNodes[i].inputs->data[j]].bytes; k++){
          printf("%d,", (int8_t)tflTensors[tflTensors_subgraph_index[g] + tflNodes[i].inputs->data[j]].data.raw[k]);
        }
      }
    }
    printf("\n");
#endif

#ifdef TFLMC_XCORE_PROFILE
#ifdef __xcore__
  asm volatile ("gettime %0" : "=r" (time_t0));
#endif
#endif

    TfLiteStatus status = registrations[used_ops[i]].invoke(&ctx, &tflNodes[i]);

#ifdef TFLMC_XCORE_PROFILE
#ifdef __xcore__
  asm volatile ("gettime %0" : "=r" (time_t1));
#endif
  op_times[used_ops[i]] += time_t1 - time_t0;
  op_counts[used_ops[i]] += 1;
  printf("\nnode %-5d %-32s %-12d", i, op_strs[used_ops[i]], time_t1 - time_t0);
#endif

#ifdef TFLMC_PRINT_TENSORS
    // print every output tensor
    printf("\n{\"node\" : \"%d\", \"op\" : \"%s\", \"data\" : [", i, op_strs[used_ops[i]]);
    for (int j=0; j<tflNodes[i].outputs->size; j++){
      printf("\n{\"tensor\" : %d, \"output\" : %d, \"bytes\" : %d, \"checksum\" : %d,\n", tflNodes[i].outputs->data[j], j, tflTensors[tflNodes[i].outputs->data[j]].bytes, checksum(tflTensors[tflNodes[i].outputs->data[j]].data.raw, tflTensors[tflNodes[i].outputs->data[j]].bytes));
      printf("\"val\" : [");
      for(int k=0; k<tflTensors[tflTensors_subgraph_index[g] + tflNodes[i].outputs->data[j]].bytes; k++){
        printf("%d", (int8_t)tflTensors[tflTensors_subgraph_index[g] + tflNodes[i].outputs->data[j]].data.raw[k]);
        if (k < tflTensors[tflTensors_subgraph_index[g] + tflNodes[i].outputs->data[j]].bytes-1){
          printf(",");
        }
      }
      if(j<tflNodes[i].outputs->size-1){
        printf("]},\n");
      } else {
        printf("]}]\n");
      }
    }

    if(i < ((tflNodes_subgraph_index[g+1] - tflNodes_subgraph_index[g]) - 1)){
      printf("},\n");
    } else {
      printf("}\n");
    }
#endif

    if (status != kTfLiteOk) {
      currentSubgraphIndex = prevSubgraphIndex;
      return status;
    }
  }
#ifdef TFLMC_PRINT_TENSORS
printf("\n]");
#endif

  currentSubgraphIndex = prevSubgraphIndex;
  return kTfLiteOk;
}

} // namespace

TfLiteTensor* model_input(int index) {
  return &ctx.tensors[inTensorIndices[index]];
}

TfLiteTensor* model_output(int index) {
  return &ctx.tensors[outTensorIndices[index]];
}

#pragma stackfunction 1000
TfLiteStatus model_init(void *weights_data_ptr) {
  // Clear and initialize
  scratch_buffer_idx = 0;
  persistentBufferPtr = tensor_arena + kTensorArenaSize;

  // Set weights data ptr in xcore context config
  xc_config.weights_data_ptr = weights_data_ptr;
  // Set thread count specified in the compiler
  xc_config.model_thread_count = 1;
  // Set thread info
  xc_config.thread_info.nstackwords = kStackWordsPerThread;
  xc_config.thread_info.stacks = &xcThreadsStack[threadsStackSizeInUint64 - 1];

  // Setup microcontext functions
  mc.AllocateTempInputTensor = &mc_AllocateTempInputTensor;
  mc.AllocateTempOutputTensor = &mc_AllocateTempOutputTensor;
  mc.DeallocateTempTfLiteTensor = &mc_DeallocateTempTfLiteTensor;
  mc.external_context = &mc_external_context;
  mc.graph = &mc_graph;

  micro_graph.NumSubgraphs = &mg_NumSubgraphs;
  micro_graph.NumSubgraphInputs = &mg_NumSubgraphInputs;
  micro_graph.NumSubgraphOutputs = &mg_NumSubgraphOutputs;
  micro_graph.GetSubgraphInput = &mg_GetSubgraphInput;
  micro_graph.GetSubgraphOutput = &mg_GetSubgraphOutput;
  micro_graph.InvokeSubgraph = &mg_InvokeSubgraph;

  // Setup tflitecontext functions
  ctx.AllocatePersistentBuffer = &AllocatePersistentBuffer;
  ctx.GetEvalTensor = &GetEvalTensor;
  ctx.RequestScratchBufferInArena = &RequestScratchBufferInArena;
  ctx.GetScratchBuffer = &GetScratchBuffer;
  
  // Set microcontext as the context ptr
  ctx.impl_ = (void*)&mc;
  ctx.tensors = tflTensors;
  ctx.tensors_size = 20;
  registrations[OP_XC_ld_weights] = *(tflite::ops::micro::xcore::Register_XC_ld_weights());
  registrations[OP_XC_conv2d_v2] = *(tflite::ops::micro::xcore::Register_XC_conv2d_v2());
  registrations[OP_XC_slice] = *(tflite::ops::micro::xcore::Register_XC_slice());
  registrations[OP_RESHAPE] = tflite::Register_RESHAPE();
  registrations[OP_XC_lookup] = *(tflite::ops::micro::xcore::Register_XC_lookup());


  // Allocate persistent buffers for variable tensors
  for (int i = 0; i < 0; i++) {
    tflTensors[varTensors_index[i]].data.data = AllocatePersistentBuffer(&ctx, tflTensors[varTensors_index[i]].bytes);
  }

#ifdef TFLMC_XCORE_PROFILE
  printf("\nProfiling init()...");
  memset(op_times, 0, sizeof(op_times));
  op_times_summed = 0;
#endif

  for(size_t g = 0; g < 1; ++g) {
    currentSubgraphIndex = g;
    for(size_t i = tflNodes_subgraph_index[g]; i < tflNodes_subgraph_index[g+1]; ++i) {
    if (registrations[used_ops[i]].init) {

#ifdef TFLMC_XCORE_PROFILE
#ifdef __xcore__
      asm volatile ("gettime %0" : "=r" (time_t0));
#endif
#endif

      tflNodes[i].user_data = registrations[used_ops[i]].init(&ctx, (const char*)tflNodes[i].builtin_data, tflNodes[i].custom_initial_data_size);

#ifdef TFLMC_XCORE_PROFILE
#ifdef __xcore__
      asm volatile ("gettime %0" : "=r" (time_t1));
#endif
      op_times[used_ops[i]] += time_t1 - time_t0;
      printf("\nnode %-5d %-32s %-12d", i, op_strs[used_ops[i]], time_t1 - time_t0);
#endif

    }
  }
  }
  currentSubgraphIndex = 0;

#ifdef TFLMC_XCORE_PROFILE
    printf("\n\nCumulative times for init()...");
    for(int i=0; i<OP_LAST; i++){
      op_times_summed += op_times[i];
      printf("\n%-32s %-12d %.2fms", op_strs[i], op_times[i], op_times[i]/100000.0);
    }
    printf("\n\nTotal time for init() - %-10lld %.2fms\n\n", op_times_summed, op_times_summed/100000.0);
  printf("\n");
  printf("\nProfiling prepare()...");
  memset(op_times, 0, sizeof(op_times));
  op_times_summed = 0;
#endif

  for(size_t g = 0; g < 1; ++g) {
        currentSubgraphIndex = g;
        for(size_t i = tflNodes_subgraph_index[g]; i < tflNodes_subgraph_index[g+1]; ++i) {
    if (registrations[used_ops[i]].prepare) {

#ifdef TFLMC_XCORE_PROFILE
#ifdef __xcore__
      asm volatile ("gettime %0" : "=r" (time_t0));
#endif
#endif

      TfLiteStatus status = registrations[used_ops[i]].prepare(&ctx, &tflNodes[i]);

#ifdef TFLMC_XCORE_PROFILE
#ifdef __xcore__
      asm volatile ("gettime %0" : "=r" (time_t1));
#endif
      op_times[used_ops[i]] += time_t1 - time_t0;
      printf("\nnode %-5d %-32s %-12d", i, op_strs[used_ops[i]], time_t1 - time_t0);
#endif

      if (status != kTfLiteOk) {
        return status;
      }
    }
  }
  }
  currentSubgraphIndex = 0;

#ifdef TFLMC_XCORE_PROFILE
printf("\n\nCumulative times for prepare()...");
    for(int i=0; i<OP_LAST; i++){
      op_times_summed += op_times[i];
      printf("\n%-32s %-12d %.2fms", op_strs[i], op_times[i], op_times[i]/100000.0);
    }
    printf("\n\nTotal time for prepare() - %-10lld %.2fms\n\n", op_times_summed, op_times_summed/100000.0);
  printf("\n");
#endif

  return kTfLiteOk;
}

#pragma stackfunction 1000
TfLiteStatus model_invoke() {
  thread_init_1(&xc_config.thread_info);

#ifdef TFLMC_XCORE_PROFILE
  printf("\nProfiling invoke()...");
  memset(op_times, 0, sizeof(op_times));
  memset(op_counts, 0, sizeof(op_counts));
  op_times_summed = 0;
#endif

  mg_InvokeSubgraph(0);

  thread_destroy(&xc_config.thread_info);

  #ifdef TFLMC_CONV2D_PROFILE
  struct convopdata{
    const char * name;
    size_t thread_count;
    int evalStartTime;
    int threadsStartTime;
    int threadsDoneTime;
  };
  int conv_times1 = 0, conv_times2 = 0;
  printf("\n\nConv()...");
  for(size_t g = 0; g < 1; ++g) {
    for(size_t i = tflNodes_subgraph_index[g]; i < tflNodes_subgraph_index[g+1]; ++i) {
      if(used_ops[i] == OP_XC_conv2d_v2) {
        auto *op_data = reinterpret_cast<convopdata *>(tflNodes[i].user_data);
        conv_times1 += op_data->threadsStartTime - op_data->evalStartTime;
        conv_times2 += op_data->threadsDoneTime - op_data->threadsStartTime;
        printf("\nnode %-5d %-25s %-25s %-6d %-6d %-12d", i, op_strs[used_ops[i]], op_data->name, op_data->thread_count, op_data->threadsStartTime - op_data->evalStartTime, op_data->threadsDoneTime - op_data->threadsStartTime);
      }
    }
  }
  printf("\nSummed - %-10d %-10d", conv_times1, conv_times2);
#endif
    
#ifdef TFLMC_XCORE_PROFILE
  printf("\n\nCumulative times for invoke()...");
  for(int i=0; i<OP_LAST; i++){
    op_times_summed += op_times[i];
    printf("\n%-5d %-32s %-12d %.2fms", op_counts[i], op_strs[i], op_times[i], op_times[i]/100000.0);
  }
  printf("\n\nTotal time for invoke() - %-10lld %.2fms\n\n", op_times_summed, op_times_summed/100000.0);
#endif

  return kTfLiteOk;
}

TfLiteStatus model_reset() {
  // Reset variable tensors
  for (int i = 0; i < 0; i++) {
    memset(tflTensors[varTensors_index[i]].data.data, tflTensors[varTensors_index[i]].params.zero_point, tflTensors[varTensors_index[i]].bytes);
  }
  return kTfLiteOk;
}

#if defined(__xcore__) && defined(USB_TILE)
#include "ioserver.h"
#include <xcore/hwtimer.h>
extern "C" {
extern int read_sswitch_reg(unsigned tile, unsigned reg, unsigned *data);
extern int write_sswitch_reg(unsigned tile, unsigned reg, unsigned data);
}

void model_ioserver(chanend_t c) {
    unsigned tensor_num = 0;
    extern unsigned tile[];
    while(1) {
        int cmd = ioserver_command_receive(c, &tensor_num);
        switch(cmd) {
        case IOSERVER_TENSOR_RECV_INPUT: {
            ioserver_tensor_recv_input(
                c, (unsigned int *) model_input(tensor_num)->data.u32,
                (model_input(tensor_num)->bytes + 3) / sizeof(int));
            break;
        }
        case IOSERVER_TENSOR_SEND_OUTPUT: {
            ioserver_tensor_send_output(
                c, (unsigned int*) model_output(tensor_num)->data.u32, 
                (model_output(tensor_num)->bytes + 3) / sizeof(int));
            break;
        }
        case IOSERVER_INVOKE: {
            model_invoke();
            ioserver_command_acknowledge(c, IOSERVER_ACK);
            break;
        }
        case IOSERVER_RESET: {
            model_reset();
            ioserver_command_acknowledge(c, IOSERVER_ACK);
            break;
        }
        case IOSERVER_EXIT: {
          ioserver_command_acknowledge(c, IOSERVER_ACK);
          unsigned pll_ctrl;
          hwtimer_t timer = hwtimer_alloc();
          hwtimer_delay(timer, 100000);
          hwtimer_free(timer);
          read_sswitch_reg(tile[USB_TILE], XS1_SSWITCH_PLL_CTL_NUM, &pll_ctrl);
          write_sswitch_reg(tile[USB_TILE], XS1_SSWITCH_PLL_CTL_NUM, pll_ctrl);
          return;
        }
        default: {
            ioserver_command_acknowledge(c, IOSERVER_NACK);
            break;
        }
        }
    }
}
#else 

void model_ioserver(void *io_channel) {}

#endif // __xcore__

