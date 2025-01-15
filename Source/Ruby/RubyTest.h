#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RubyTest.generated.h"

UCLASS()
class RUBY_API URubyTest : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
    UFUNCTION(BlueprintCallable, Category="Ruby")
    static void RubyCodeInit();

    UFUNCTION(BlueprintCallable, Category="Ruby")
    static void RubyCode();

    UFUNCTION(BlueprintCallable, Category="Ruby")
    static float RubyFunc(float a, float b);
};
