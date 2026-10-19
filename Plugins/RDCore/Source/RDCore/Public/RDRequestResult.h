// The answer to any request ("switch state", later "cast this spell"...):
// accepted or refused, a tag saying why it was refused, and readable text for logs/debug.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RDRequestResult.generated.h"

USTRUCT(BlueprintType)
struct RDCORE_API FRDRequestResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Rediscovery")
	bool bAccepted = false;

	// Empty when accepted. Otherwise one of the RD.Refusal.* tags (or a module-specific one).
	UPROPERTY(BlueprintReadOnly, Category = "Rediscovery")
	FGameplayTag RefusalReason;

	UPROPERTY(BlueprintReadOnly, Category = "Rediscovery")
	FString DebugText;

	static FRDRequestResult Accept(const FString& InText = FString())
	{
		FRDRequestResult R;
		R.bAccepted = true;
		R.DebugText = InText;
		return R;
	}

	static FRDRequestResult Refuse(const FGameplayTag& InReason, const FString& InText)
	{
		FRDRequestResult R;
		R.bAccepted = false;
		R.RefusalReason = InReason;
		R.DebugText = InText;
		return R;
	}
};
