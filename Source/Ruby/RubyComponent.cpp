#include "RubyComponent.h"
#include "Ruby.h"
#include "mrubyc.h"
#include "opcode.h"

extern const uint8_t mrbbuf[];

URubyComponent::URubyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    UE_LOG(LogTemp, Log, TEXT("URubyComponent"));
}


void URubyComponent::StartRuby()
{
	if (RubyStarted || StartError)
		return;

    UE_LOG(LogTemp, Log, TEXT("StartRuby"));

    // run SourcePath through mrb
    // put the output in ProjectIntermediateDir/Ruby/

	IFileManager& fmgr = IFileManager::Get();
	FString dest_dir = FPaths::ProjectIntermediateDir() + "Ruby/";
	if (!fmgr.DirectoryExists(*dest_dir))
		fmgr.MakeDirectory(*dest_dir, true);

    FString ignoreme, filename, extension;
	FPaths::Split(SourcePath, ignoreme, filename, extension);
	FString dest_path = dest_dir + filename + ".mrb";
	FString parms = "-o \"" + dest_path + "\" \"" + SourcePath + "\"";
    // UE_LOG(LogTemp, Log, TEXT("StartRuby - %s %s"), *BinPath, *parms);

	double t = FPlatformTime::Seconds();

    int32 rc;
    FString out, err;
	bool exec_res = FMacPlatformProcess::ExecProcess(*BinPath, *parms, &rc, &out, &err, nullptr, false);
	t = FPlatformTime::Seconds() - t;
    UE_LOG(LogTemp, Log, TEXT("StartRuby - exec time: %f"), t);

	if (!exec_res)
	{
	    UE_LOG(LogTemp, Log, TEXT("StartRuby - mrbc not invoked"));
	    StartError = 10;
		return;
	}
    // UE_LOG(LogTemp, Log, TEXT("StartRuby - ReturnCode %d"), rc);
    if (rc == 1)
    {
	    UE_LOG(LogTemp, Log, TEXT("StartRuby - STDOUT %s"), *out);
	    UE_LOG(LogTemp, Log, TEXT("StartRuby - STDERR %s"), *err);
	    StartError = 15;	    
    }

	// load mrb
	if (!fmgr.FileExists(*dest_path)) {
	    UE_LOG(LogTemp, Log, TEXT("StartRuby - mrbc no output file"));
	    StartError = 20;
		return;
	}

	MemoryPool = FMemory::Malloc(MRBC_MEMORY_SIZE);
	mrbc_init_alloc((void *) MemoryPool, MRBC_MEMORY_SIZE);
	mrbc_init_global();
	mrbc_init_class();

	VM = mrbc_vm_open(NULL);
	if (VM == NULL) {
	    UE_LOG(LogTemp, Log, TEXT("StartRuby mrbc_vm_open failed"));
	    StartError = 40;
		return;
	}

	TArray<uint8> mrb_code;
	bool res = FFileHelper::LoadFileToArray(mrb_code, *dest_path);
	if (!res) {
	    UE_LOG(LogTemp, Log, TEXT("StartRuby - LoadFileToArray fail"));
	    StartError = 30;
		return;
	}
	Code = FMemory::Malloc(mrb_code.Num());
	memcpy(Code, mrb_code.GetData(), mrb_code.Num());

	if (mrbc_load_mrb(VM, Code) != 0) {
	    UE_LOG(LogTemp, Log, TEXT("StartRuby mrbc_load_mrb failed"));
	    StartError = 50;
		return;
	}

	mrbc_vm_begin(VM);
	int ret = mrbc_vm_run(VM);

	ComponentClass = mrbc_get_class_by_name("TestRubyComponent");
	if (!ComponentClass) {
	    UE_LOG(LogTemp, Log, TEXT("StartRuby ComponentClass missing"));
	    StartError = 60;
		return;
	}
	ComponentInstance = mrbc_instance_new(VM, ComponentClass, 0);

	StartTime = FPlatformTime::Seconds();
	RubyStarted = true;
}


void URubyComponent::StopRuby()
{
	if (!RubyStarted)
		return;
	RubyStarted = false;

    UE_LOG(LogTemp, Log, TEXT("StopRuby"));

	mrbc_vm_end(VM);
	mrbc_vm_close(VM);

	mrbc_cleanup_alloc();
	mrbc_cleanup_vm();
	mrbc_cleanup_symbol();

	FMemory::Free(MemoryPool);
	MemoryPool = NULL;
	FMemory::Free(Code);
	Code = NULL;
}


void URubyComponent::Thunk(const char *method_name)
{
	mrbc_method method;
	int method_sym_id;

	if (!ComponentClass)
		return;

	method_sym_id = mrbc_str_to_symid(method_name);
	if (method_sym_id == -1)
		return;

  	if (mrbc_find_method(&method, ComponentClass, method_sym_id) == 0)
  		return;

	mrbc_vm_begin(VM);
	mrbc_vm_run_method(VM, &ComponentInstance, &method);
}


void URubyComponent::BeginPlay()
{
	Super::BeginPlay();

	StartRuby();
	if (!RubyStarted)
		return;
	Thunk("beginPlay");
}


void URubyComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

	StartRuby();
	if (!RubyStarted)
		return;
	Thunk("endPlay");
    StopRuby();
}


void URubyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	StartRuby();
	if (!RubyStarted)
		return;

	Thunk("tickComponent");
}
