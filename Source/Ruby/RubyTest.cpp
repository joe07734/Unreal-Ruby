#include "RubyTest.h"
#include "Ruby.h"
#include "mrubyc.h"

static const uint8_t mrbbuf1[] = {
0x52,0x49,0x54,0x45,0x30,0x33,0x30,0x30,0x00,0x00,0x00,0xbf,0x4d,0x41,0x54,0x5a,
0x30,0x30,0x30,0x30,0x49,0x52,0x45,0x50,0x00,0x00,0x00,0x90,0x30,0x33,0x30,0x30,
0x00,0x00,0x00,0x5c,0x00,0x01,0x00,0x04,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x1b,
0x51,0x02,0x00,0x2d,0x01,0x00,0x01,0x03,0x01,0x0a,0x57,0x02,0x00,0x30,0x01,0x01,
0x00,0x51,0x02,0x01,0x2d,0x01,0x00,0x01,0x38,0x01,0x69,0x00,0x02,0x00,0x00,0x0c,
0x6d,0x72,0x75,0x62,0x79,0x63,0x20,0x73,0x74,0x61,0x72,0x74,0x00,0x00,0x00,0x0a,
0x6d,0x72,0x75,0x62,0x79,0x63,0x20,0x65,0x6e,0x64,0x00,0x00,0x02,0x00,0x04,0x70,
0x75,0x74,0x73,0x00,0x00,0x05,0x74,0x69,0x6d,0x65,0x73,0x00,0x00,0x00,0x00,0x28,
0x00,0x03,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0d,0x34,0x04,0x00,0x00,
0x01,0x04,0x01,0x2d,0x03,0x00,0x01,0x38,0x03,0x00,0x00,0x00,0x01,0x00,0x04,0x70,
0x75,0x74,0x73,0x00,0x4c,0x56,0x41,0x52,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x01,
0x00,0x01,0x69,0x00,0x00,0xff,0xff,0x45,0x4e,0x44,0x00,0x00,0x00,0x00,0x08
};


void URubyTest::RubyCodeInit()
{
	// mrbc_init_alloc((void *) memory_pool, MRBC_MEMORY_SIZE);
	// mrbc_init_global();
	// mrbc_init_class();
}


void URubyTest::RubyCode()
{
    UE_LOG(LogTemp, Log, TEXT("RubyCode begin"));

	mrbc_vm *vm = mrbc_vm_open(NULL);
	if( vm == NULL ) {
	return;
	}

	if( mrbc_load_mrb(vm, mrbbuf1) != 0 ) {
	return;
	}

	mrbc_vm_begin( vm );
	int ret = mrbc_vm_run( vm );
	mrbc_vm_end( vm );
	mrbc_vm_close( vm );

    UE_LOG(LogTemp, Log, TEXT("RubyCode done"));
}


float URubyTest::RubyFunc(float a, float b)
{
    UE_LOG(LogTemp, Log, TEXT("RubyFunc begin"));

	// mrbc_vm *vm = mrbc_vm_open(NULL);
	// if( vm == NULL ) {
	// return;
	// }

	// if( mrbc_load_mrb(vm, mrbbuf2) != 0 ) {
	// return;
	// }

	// mrbc_vm_begin( vm );
	// int ret = mrbc_vm_run( vm );
	// mrbc_vm_end( vm );
	// mrbc_vm_close( vm );

    UE_LOG(LogTemp, Log, TEXT("RubyFunc done"));
    return 0;
}
