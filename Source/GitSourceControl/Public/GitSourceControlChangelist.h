#pragma once
#include "ISourceControlChangelist.h"
#include "Misc/EngineVersionComparison.h"
#include "Runtime/Launch/Resources/Version.h"

class FGitSourceControlChangelist : public ISourceControlChangelist
{
public:
	FGitSourceControlChangelist() = default;

	explicit FGitSourceControlChangelist(FString&& InChangelistName, const bool bInInitialized = false)
		: ChangelistName(MoveTemp(InChangelistName))
		  , bInitialized(bInInitialized)
	{
	}

	virtual bool CanDelete() const override
	{
		return false;
	}

	bool operator==(const FGitSourceControlChangelist& InOther) const
	{
		return ChangelistName == InOther.ChangelistName;
	}

	bool operator!=(const FGitSourceControlChangelist& InOther) const
	{
		return ChangelistName != InOther.ChangelistName;
	}

#if !UE_VERSION_OLDER_THAN(5, 3, 0)
	virtual bool IsDefault() const override
	{
		return ChangelistName == WorkingChangelist.ChangelistName;
	}
#endif

	void SetInitialized()
	{
		bInitialized = true;
	}

	bool IsInitialized() const
	{
		return bInitialized;
	}

	void Reset()
	{
		ChangelistName.Reset();
		bInitialized = false;
	}

	friend FORCEINLINE uint32 GetTypeHash(const FGitSourceControlChangelist& InGitChangelist)
	{
		return GetTypeHash(InGitChangelist.ChangelistName);
	}

	FString GetName() const
	{
		return ChangelistName;
	}

#if !UE_VERSION_OLDER_THAN(5, 3, 0)
	virtual FString GetIdentifier() const override
	{
		return ChangelistName;
	}
#endif

public:
	static FGitSourceControlChangelist WorkingChangelist;
	static FGitSourceControlChangelist StagedChangelist;

private:
	FString ChangelistName;
	bool bInitialized = false;
};

typedef TSharedRef<class FGitSourceControlChangelist, ESPMode::ThreadSafe> FGitSourceControlChangelistRef;
