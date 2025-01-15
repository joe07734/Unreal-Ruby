#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "mrubyc.h"
#include "RubyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RUBY_API URubyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URubyComponent();

protected:
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Source", Category = "Ruby")
    FString SourcePath = "/Users/joe/Unreal-Ruby/Source/Ruby/TestRubyComponent.rb";

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Path to mrbc", Category = "Ruby")
    FString BinPath = "/Users/joe/mruby/bin/mrbc";

    UPROPERTY(EditAnywhere, BlueprintReadOnly, DisplayName="Print to Game", Category = "Ruby")
    bool PrintToGame = true;

private:
	void StartRuby();
	void StopRuby();
	void Thunk(const char *method_name);

	double StartTime;
	bool RubyStarted = false;
	int StartError = 0;

	void *MemoryPool;
	mrbc_vm *VM;
	void *Code;
	mrbc_class *ComponentClass;  /* Ruby class containing this component */
	mrbc_value ComponentInstance;  /* Ruby instance mirroring this component */
};

#define MRBC_MEMORY_SIZE (1024*40)
