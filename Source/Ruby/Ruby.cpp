#include "Ruby.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Ruby, "Ruby" );

#include "mrubyc.h"

uint8_t _PrintToGame = 1;
static const FColor RubyColor = FColor(211,49,25);

extern "C" {
void unreal_mrbc_out_of_memory(void) {
    UE_LOG(LogTemp, Log, TEXT("unreal_mrbc_out_of_memory"));
}


static FString write_buffer;

int unreal_hal_write(int fd, const void *buf, int nbytes) {
	write_buffer += FString::ConstructFromPtrSize((const UTF8CHAR *)buf, nbytes);

	if (write_buffer.Right(1) == "\n") {
	    UE_LOG(LogRuby, Log, TEXT("%s"), *write_buffer);
	    if (_PrintToGame) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, RubyColor, write_buffer.Left(write_buffer.Len() - 1));
	    }
	    write_buffer = "";
	}
    return 0;
}

int unreal_hal_flush(int fd) {
	if (write_buffer.Len() > 0)
	{
		static uint8_t cr[2] = {"\n"};
		unreal_hal_write(fd, cr, 1);
	}
    return 0;
}
}  // extern "C"



